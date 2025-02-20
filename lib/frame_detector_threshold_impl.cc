/**
 * @file frame_detector_threshold_impl.cc
 * @author Martyn van Dijke (martijnvdijke600@gmail.com)
 * @brief
 * @version 0.3
 * @date 2021-03-23
 *
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "frame_detector_threshold_impl.h"
#include "helpers.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lora_sdr {

frame_detector_threshold::sptr frame_detector_threshold::make(uint8_t sf, uint32_t samp_rate,
                                          uint32_t bw, float threshold) {
  return gnuradio::get_initial_sptr(
      new frame_detector_threshold_impl(sf, samp_rate, bw, threshold));
}

/**
 * @brief Construct a new frame detector impl object
 *
 * @param samp_rate : sampling rate
 * @param bandwidth : bandwith
 * @param sf : spreading factor
 */
frame_detector_threshold_impl::frame_detector_threshold_impl(uint8_t sf, uint32_t samp_rate,
                                         uint32_t bw, float threshold)
    : gr::block("frame_detector_threshold",
                gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(1, 1, sizeof(gr_complex))) {
  gr::block::set_thread_priority(90);
  // set internal variables
  // spreading factor
  m_sf = sf;
  m_samp_rate = samp_rate;
  m_bw = bw;
  //  int temp = pow(2,(m_sf-7));
  // threshold value -> since
  m_threshold = threshold;
  // calculate derived variables number of samples per symbol
  m_N = (uint32_t)(1u << m_sf);
  m_symbols_per_second = (double)m_bw / m_N;
  m_samples_per_symbol = (uint32_t)(m_samp_rate / m_symbols_per_second);

  // initialise all vector values and make sure they have the same length and are initialized
  fft_cfg = kiss_fft_alloc(m_N, 0, NULL, NULL);
  cx_out.resize(m_N, 0.0);
  m_dfts_mag.resize(m_N, 0);
  m_dechirped.resize(m_N, 0);
  m_temp.resize(m_N, 0);
  // set downchirp and generate downchirp
  m_downchirp.resize(m_N);
  for (uint n = 0; n < m_N; n++) {
    m_downchirp[n] = gr_expj(-2.0 * M_PI * (pow(n, 2) / (2 * m_N) - 0.5 * n));
  }
  // number of consecutive up chrips a preamble has
  n_up = 8;

  // set buffer reserve
  buffer.reserve(m_N * n_up);

  // initialize values of variables
  bin_idx = 0;
  symbol_cnt = 0;
  m_power = 0;
  // set initial state to find preamble
  m_state = FIND_PREAMBLE;
  m_cnt = 0;
  in_frame = false;
  m_inter_frame_padding = 1;
  cnt_padding = 0;
  m_end_offset = m_samples_per_symbol*10000;
  m_n_avg = 1;

#ifdef GRLORA_FILE
        file_ratio.open("../test/ratio.csv",
                         std::ios::out | std::ios::trunc);
        file_ratio << "First line \n";
#endif

  //set tag detection initial values.
  m_detected_tag_begin = false;
  m_detected_tag_end = false;

  // set tag propagation
  set_tag_propagation_policy(TPP_ONE_TO_ONE);
}

/**
 * @brief Get the symbol object value (aka decoded LoRa symbol value)
 *
 * @param input : complex samples
 * @return int32_t : LoRa symbol value
 */
int32_t frame_detector_threshold_impl::get_symbol_val(const gr_complex *input) {
  // dechirp the new potential symbol
  volk_32fc_x2_multiply_32fc(&m_dechirped[0], input, &m_downchirp[0], m_N);
  // do the FFT
  kiss_fft(fft_cfg, (kiss_fft_cpx *)&m_dechirped[0],
           (kiss_fft_cpx *)&cx_out[0]);
  // get abs value of each fft value
  for (int i = 0; i < m_N; i++) {
    m_dfts_mag[i] = std::abs(cx_out[i]);
  }
  // get the maximum element from the fft values
  m_max_it = std::max_element(m_dfts_mag.begin(), m_dfts_mag.end());
  int32_t arg_max = std::distance(m_dfts_mag.begin(), m_max_it);
  // return the maximum dft bin containing the LoRa symbol
  return arg_max;
}

/**
 * @brief Checks if current samples have the right
 *
 * @param input
 * @return true : we are in a LoRa frame
 * @return false : we are not in a LoRa frame
 */
bool frame_detector_threshold_impl::check_in_frame(const gr_complex *input) {
  // compare power to compare against
  float compare_power = 0;
  float current_power = 0;
  current_power = frame_detector_threshold_impl::calc_power(input);
  // get current power
  compare_power = m_power - m_threshold;

  //push the ratio of power to the vector for taking the average
//  avg_ratio.push_back(current_power);
//  //if average vector has enough items
//  if(avg_ratio.size() > m_n_avg){
//      //calulate avg
//      current_power = std::accumulate(avg_ratio.begin(), avg_ratio.end(), 0.0)/avg_ratio.size();
//      //delete first item from vector
//      avg_ratio.erase(avg_ratio.begin());
////      std::cout << "AVG ratio" +  std::to_string(current_power) << std::endl;
////
////    for(int i =0; k < avg_ratio.size(); i++){
////        avg_power += avg_ratio.at(i);
////    }
//
//  }

  // if current power is higher then the set power - threshold we are in the
  // LoRa frame
  if (current_power > compare_power) {
    return true;
  } else {
#ifdef GRLORA_DEBUGV
    GR_LOG_DEBUG(this->d_logger,
                 "DEBUG:Outside LoRa frame, current power:" + std::to_string(current_power) +
                     " compare power:" + std::to_string(compare_power));
#endif
    // we are not inside safe margin of threshold (not in a LoRa frame)
    return false;
  }
}

/**
 * @brief Calculates the LoRa frame peak power
 *
 * @param input : input samples
 * @return float : peak power
 */
float frame_detector_threshold_impl::calc_power(const gr_complex *input) {
  // temporary variables
  // variable to hold the signal power
  float signal_power = 0;
  // number of bins to check around (3 for +-1)
  int n_bin = 3;
  // get maximum argument
  int32_t arg_max = get_symbol_val(input);

  // calculate power around peak +-1 symbol
  for (int j = -n_bin / 2; j <= n_bin / 2; j++) {
    signal_power += std::abs(m_dfts_mag[(arg_max + j)]);
  }

  float peak_power = 0;
  peak_power = signal_power;
  // divide by three to compensate for the +-1 bins
  signal_power = signal_power / 3;
#ifdef GRLORA_DEBUGv
        GR_LOG_DEBUG(this->d_logger,
                 "DEBUG:signal power:" + std::to_string(signal_power));
#endif
        if( signal_power > 4000.0){
            GR_LOG_CRIT(this->d_logger,"CRIT:To much signal power, should not happen");
            signal_power = 0;
        }

  // loop over the entire dft spectrum and sum the power of all noise
  float noise_power = 0;

  unsigned int alignment = volk_get_alignment();
  float *out = (float *)volk_malloc(sizeof(float), m_N);
  volk_32f_accumulator_s32f(out, &m_dfts_mag[0], m_N);
  // disregard the power of the peak signal and divide by the length
  noise_power = (*out - peak_power) / ((float)(m_N - n_bin));
//#ifdef GRLORA_DEBUG
//        GR_LOG_DEBUG(this->d_logger,
//                 "DEBUG:noise power:" + std::to_string(noise_power));
//#endif
  if (noise_power > 1) {
    signal_power = signal_power / noise_power;
  } else {
    signal_power = signal_power;
  }
#ifdef GRLORA_DEBUGv
        GR_LOG_DEBUG(this->d_logger,
                 "DEBUG:ratio power:" + std::to_string(signal_power)+" at position in stream: "+std::to_string(nitems_read(0)) );
#endif
#ifdef GRLORA_FILE
  file_ratio << signal_power << ",";
#endif
  volk_free(out);
  return signal_power;
}

/**
 * @brief Set the current LoRa frame power
 *
 * @param input : complex samples
 */
void frame_detector_threshold_impl::set_power(const gr_complex *input) {
  m_power = calc_power(input);
}

/**
 * @brief Destroy the frame detector impl::frame detector impl object
 *
 */
frame_detector_threshold_impl::~frame_detector_threshold_impl() {}

/**
 * @brief
 *
 * @param noutput_items : number of output items
 * @param ninput_items_required : required input items (how many items must we
 * have for we can do something)
 */
void frame_detector_threshold_impl::forecast(int noutput_items,
                                   gr_vector_int &ninput_items_required) {
  /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
  if (m_state == FIND_PREAMBLE) {
    // we need at least the preamble symbols to start working
    ninput_items_required[0] = m_samples_per_symbol;
  }
  if (m_state == SEND_PREAMBLE) {
    // we are just emptying our internal buffer, we do not need any new input
    ninput_items_required[0] = 0;
  }
  if (m_state == SEND_FRAME) {
    // we process the input based on the number of samples in a symbol, so we
    // only need one symbol.
    ninput_items_required[0] = m_samples_per_symbol;
  }
}

/**
 * @brief General work function.
 * Main gnuradio function that does the heavy lifting
 *
 * @param noutput_items : number of output items
 * @param ninput_items : number of input items
 * @param input_items : input items
 * @param output_items : output items
 * @return int
 */
int frame_detector_threshold_impl::general_work(int noutput_items,
                                      gr_vector_int &ninput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items) {
  const gr_complex *in = (const gr_complex *)input_items[0];
  gr_complex *out = (gr_complex *)output_items[0];

#ifdef GRLORA_SIM
        //Search for tags in the data stream and if the end tag is found save it
        get_tags_in_window(m_tags_vector,0, 0, ninput_items[0],
                          pmt::string_to_symbol("frame"));
        if (m_tags_vector.size()) {
            for(int j=0; j< m_tags_vector.size(); j++){
              if (pmt::symbol_to_string(m_tags_vector[j].value) == "end"){
              m_end_offset = m_tags_vector[j].offset;
              }
              if (pmt::symbol_to_string(m_tags_vector[j].value) == "start"){
              m_begin_offset = m_tags_vector[j].offset;
              }
            }
        }
#endif
  // search for work_done tags and if found add them to the stream
  std::vector<tag_t> work_done_tags;
  get_tags_in_window(work_done_tags, 0, 0, ninput_items[0],
                     pmt::string_to_symbol("work_done"));
  if (work_done_tags.size()) {
    add_item_tag(0, nitems_written(0), pmt::intern("work_done"),
                 pmt::intern("done"), pmt::intern("frame_detector_threshold"));
    consume_each(ninput_items[0]);
    out[0] = gr_complex(0.0,0.0);
    return 1;
  }
        switch (m_state) {
  case FIND_PREAMBLE: {
    // copy input to memory vector for later use.
    for (int i = 0; i < m_N; i++) {
      buffer.push_back(in[i]);
    }
    // tell scheduler how many items have been used
    consume_each(m_N);

    // get symbol value of input
    bin_idx_new = get_symbol_val(&in[0]);
    #ifdef GRLORA_DEBUGV
//      GR_LOG_DEBUG(this->d_logger, "DEBUG:Symbol:"+std::to_string(bin_idx_new));
#endif

    // calculate difference between this value and previous symbol value
    if (std::abs(bin_idx_new - bin_idx) <= 1 &&
        bin_idx_new != -1) {
      // increase the number of symbols counted
      symbol_cnt++;
    }
    // is symbol value are not close to each other start over
    else {
      // clear memory vector
      buffer.clear();
      bin_idx = bin_idx_new;
      // set symbol value to be 1
      symbol_cnt = 1;
    }
    // number of preambles needed
    int nR_up = (int)(n_up);
    // if we have n_up-1 symbols counted we have found the preamble
    if (symbol_cnt == nR_up) {
#ifdef GRLORA_DEBUGV
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Found PREAMBLE -> SEND PREAMBLE!");
#endif
#ifdef GRLORA_SIM
    m_begin_store = nitems_written(0);
//    if( (nitems_read(0)-n_up*m_N ) <= m_begin_offset.at(0)  ){
//          GR_LOG_DEBUG(this->d_logger, "DEBUG:Found beginning tag correctly:"+std::to_string(m_begin_offset.at(0)));
//          m_detected_tag_begin = true;
//          symbol_cnt = 0;
//          //pop first item
//          if(m_begin_offset.size() > 1) {
//              m_begin_offset.erase(m_begin_offset.begin());
//          }
//      }else{
//          GR_LOG_DEBUG(this->d_logger, "DEBUG:Found no beginning tag:"+std::to_string((nitems_read(0)-n_up*m_N )));
//          GR_LOG_DEBUG(this->d_logger, "DEBUG:beginning tag is at:"+std::to_string(m_begin_offset.at(0)));
//      }
//      //find the frame info tag
//        get_tags_in_range(m_tags_vector,0, nitems_written(0), nitems_read(0)+m_samples_per_symbol,
//                           pmt::string_to_symbol("frame"));
//        if (m_tags_vector.size()) {
//            if(pmt::symbol_to_string(m_tags_vector[0].value) == "start"){
//#ifdef GRLORA_DEBUGV
//                GR_LOG_DEBUG(this->d_logger, "DEBUG:Found correct beginning tag at offset:"+std::to_string(m_tags_vector[0].offset));
//#endif
//                m_detected_tag_begin = true;
//            }
//        }
//        GR_LOG_DEBUG(this->d_logger, "DEBUG:Found no beginning tag:"+std::to_string(nitems_written(0)));

#endif
      // store the current power level in m_power
      set_power(&in[0]);
      // set state to be sending LoRa frame packets
      m_state = SEND_PREAMBLE;

      #ifdef GRLORA_DEBUGV
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Tagging start of frame at :" +
                                       std::to_string(nitems_written(0)));
      GR_LOG_DEBUG(this->d_logger, "DEBUG:set power at:" +
                                       std::to_string(m_power));
#endif
//      add_item_tag(0, nitems_written(0), pmt::intern("frame"),
//                   pmt::intern("start"), pmt::intern("frame_detector_threshold"));

      // set symbol count back to zero
      symbol_cnt = 0;
      return 0;
    }
    return 0;
  }
  case SEND_PREAMBLE: {
    // send the preamble symbols
#ifdef GRLORA_DEBUGV
    GR_LOG_DEBUG(this->d_logger, "DEBUG:Starting sending preamble");
#endif
    // set the end of the vector to be or the maximum number of items we can
    // output or the maximum of the vector
    int end_vec = buffer.size();
    if (end_vec > noutput_items) {
      end_vec = noutput_items;
    }

    // set output to be the buffer
    // TODO could maybe use memcpy for speed
    for (int i = 0; i < end_vec; i++) {
      out[i] = buffer.at(i);
#ifdef GRLORA_log
      output_log_after << out[i] << std::endl;
#endif
    }

    // clear used items from buffer
    buffer.erase(buffer.begin(), buffer.begin() + end_vec);
    if (buffer.empty()) {
      // go to sending the rest of the symbols
      m_state = SEND_FRAME;
#ifdef GRLORA_DEBUGV
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Done SEND_PREAMBLE -> SEND_FRAME");
#endif
    }
    // tell the gnuradio scheduler how many items we have used.
    consume_each(0);
    // return the number of items produced by the system
    return end_vec;
  }

  case SEND_FRAME: {
    // Computing power of input and checking if this is below the preamble power
    // - threshold

    if (noutput_items < m_samples_per_symbol) {
      consume_each(0);
      return 0;
    }
    if (m_cnt < 10) {
      // after we have found the preamble there are 5 symbols containing the
      // network identifiers and
      // downchrips we skip these in calculation because they make
      // life/computations hard and are always there in the frame
      in_frame = true;
    } else {
      // if we are past the symbols containing network and downchirps
      // check if we are still in the frame
      in_frame = check_in_frame(&in[0]);
    }
    // increment proceed symbol counter
    m_cnt++;
    // tell the gnuradio scheduler how many items we have used.
    // if we are still in a frame
    if (in_frame == true) {
      consume_each(m_samples_per_symbol);

      // copy input to output
      memcpy(&out[0], &in[0], sizeof(gr_complex) * m_samples_per_symbol);
      // return the number of items produced by the system
      return m_samples_per_symbol;
    } else {
      consume_each(0);
#ifdef GRLORA_SIM
        if(nitems_read(0)>= m_end_offset) {
#ifdef GRLORA_DEBUGV
            GR_LOG_DEBUG(this->d_logger, "DEBUG:Correct end tag is at :"+std::to_string(m_end_offset));
            GR_LOG_DEBUG(this->d_logger, "DEBUG:Frame detector is at :"+std::to_string(nitems_read(0)));
#endif

m_detected_tag_end = true;
        }
        if( m_begin_store <= m_begin_offset){
        m_detected_tag_begin =true;
#ifdef GRLORA_DEBUGV
      GR_LOG_DEBUG(this->d_logger, "DEBUG:begin tag is at :"+std::to_string(m_begin_offset));
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Frame detector is at :"+std::to_string(m_begin_store));
#endif
        }
        else{
            GR_LOG_DEBUG(this->d_logger, "DEBUG:Correct end tag is at :"+std::to_string(m_end_offset));
            GR_LOG_DEBUG(this->d_logger, "DEBUG:Frame detector is at :"+std::to_string(nitems_read(0)));

        }

#endif
      m_state = SEND_END_FRAME;

#ifdef GRLORA_DEBUGV
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Done SEND_FRAME -> SEND_END_FRAME");
#endif

      // we consume items but do not output any items (if its outside the frame)
      // return the number of items produced by the system
      return 0;
    }
  }
  case SEND_END_FRAME: {

    if (cnt_padding == m_inter_frame_padding) {
      consume_each((m_samples_per_symbol / 4));
      memcpy(&out[0], &in[0], sizeof(gr_complex) * (m_samples_per_symbol / 4));
      cnt_padding = 0;
      // initialize values of variables
      symbol_cnt = 0;
      m_power = 0;
      // set initial state to find preamble
      m_cnt = 0;
      m_state = FIND_PREAMBLE;
#ifdef GRLORA_SIM
      if(m_detected_tag_begin && m_detected_tag_end){
          GR_LOG_DEBUG(this->d_logger,
                       "DEBUG:Good Packet Detected");
      } else{
          GR_LOG_DEBUG(this->d_logger,
                       "DEBUG:Packet Detection Error");
          GR_LOG_DEBUG(this->d_logger,
                       "DEBUG:detected begin :"+std::to_string(m_detected_tag_begin));
          GR_LOG_DEBUG(this->d_logger,
                       "DEBUG:detected end :"+std::to_string(m_detected_tag_end));
      }
      m_detected_tag_begin = false;
      m_detected_tag_end = false;
#endif

#ifdef GRLORA_DEBUGV
      GR_LOG_DEBUG(this->d_logger,
                   "DEBUG:Done SEND_END_FRAME -> FIND_PREAMBLE");
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Tagging end of frame at :" +
                                       std::to_string(nitems_written(0)+(m_samples_per_symbol / 4)));
#endif
//      add_item_tag(0, nitems_written(0) + (m_samples_per_symbol / 4),
//                   pmt::intern("frame"), pmt::intern("end"),
//                   pmt::intern("frame_detector_threshold"));
      return (m_samples_per_symbol / 4);
    } else {
      // copy the 4 inter padding frames to the output
      consume_each(m_samples_per_symbol);
      // copy input to output
      memcpy(&out[0], &in[0], sizeof(gr_complex) * m_samples_per_symbol);
      // return the number of items produced by the system
      cnt_padding++;
      return m_samples_per_symbol;
    }
  }

  default: {
    GR_LOG_WARN(this->d_logger, "WARNING : No state! Shouldn't happen");
    return 0;
  }
  }
}

} /* namespace lora_sdr */
} /* namespace gr */