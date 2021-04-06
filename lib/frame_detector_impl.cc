/**
 * @file frame_detector_impl.cc
 * @author Martyn van Dijke (martijnvdijke600@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-03-23
 *
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "frame_detector_impl.h"
#include "helpers.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lora_sdr {

frame_detector::sptr frame_detector::make(float samp_rate, uint32_t bandwidth,
                                          uint8_t sf) {
  return gnuradio::get_initial_sptr(
      new frame_detector_impl(samp_rate, bandwidth, sf));
}

/**
 * @brief Construct a new frame detector impl object
 *
 * @param samp_rate : sampling rate
 * @param bandwidth : bandwith
 * @param sf : spreading factor
 */
frame_detector_impl::frame_detector_impl(float samp_rate, uint32_t bandwidth,
                                         uint8_t sf)
    : gr::block("frame_detector",
                gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(1, 1, sizeof(gr_complex))) {
  // set internal variables
  m_bw = bandwidth;
  m_samp_rate = samp_rate;
  m_sf = sf;
  // set to default values for now
  m_os_factor = 1;
  m_margin = 1;
  m_fft_symb = 4;
  m_threshold = 2000;

  // calculate deducted variables.
  m_N = (uint32_t)(1u << m_sf);
  m_samples_per_symbol = (uint32_t)(m_samp_rate * m_N / m_bw);

  fft_cfg = kiss_fft_alloc(m_N * m_fft_symb, 0, NULL, NULL);
  cx_out.resize(m_N * m_fft_symb, 0.0);
  m_input_decim.resize(m_N, 0);
  m_dfts_mag.resize(m_N * m_fft_symb, 0);
  m_dechirped.resize(m_N * m_fft_symb, 0);

  m_downchirp.resize(m_N);
  // generate reference downchirp
  for (uint n = 0; n < m_N; n++) {
    m_downchirp[n] = gr_expj(-2.0 * M_PI * (pow(n, 2) / (2 * m_N) - 0.5 * n));
  }

  // number of consecutive up chrips a preamble has
  n_up = 8;

  // initialize values of variables
  bin_idx = 0;
  symbol_cnt = 0;

  // set initial state to find preamble
  m_state = FIND_PREAMBLE;
}

/**
 * @brief Destroy the frame detector impl::frame detector impl object
 *
 */
frame_detector_impl::~frame_detector_impl() {}

/**
 * @brief
 *
 * @param noutput_items : number of output items
 * @param ninput_items_required : required input items (how many items must we
 * have for we can do something)
 */
void frame_detector_impl::forecast(int noutput_items,
                                   gr_vector_int &ninput_items_required) {
  /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
  // we need at least the preamble symbols to start working
  ninput_items_required[0] = n_up * m_samples_per_symbol;
}

//uint32_t get_symbol_val(const gr_complex *samples, gr_complex *ref_chirp,
//                        uint32_t m_number_of_bins,
//                        uint32_t m_samples_per_symbol, kiss_fft_cpx *cx_in,
//                        kiss_fft_cpx *cx_out) {
//  double sig_en = 0;
//  float fft_mag[m_number_of_bins];
//  std::vector<gr_complex> dechirped(m_number_of_bins);
//
//  kiss_fft_cfg cfg = kiss_fft_alloc(m_samples_per_symbol, 0, 0, 0);
//
//  // Multiply with ideal downchirp
//  volk_32fc_x2_multiply_32fc(&dechirped[0], samples, ref_chirp,
//                             m_samples_per_symbol);
//
//  for (int i = 0; i < m_samples_per_symbol; i++) {
//    cx_in[i].r = dechirped[i].real();
//    cx_in[i].i = dechirped[i].imag();
//  }
//  // do the FFT
//  kiss_fft(cfg, cx_in, cx_out);
//
//  // Get magnitude
//  for (uint32_t i = 0u; i < m_number_of_bins; i++) {
//    fft_mag[i] = cx_out[i].r * cx_out[i].r + cx_out[i].i * cx_out[i].i;
//    sig_en += fft_mag[i];
//  }
//  free(cfg);
//  // Return argmax here
//  return ((std::max_element(fft_mag, fft_mag + m_number_of_bins) - fft_mag));
//}

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
int frame_detector_impl::general_work(int noutput_items,
                                      gr_vector_int &ninput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items) {
  const gr_complex *in = (const gr_complex *)input_items[0];
  gr_complex *out = (gr_complex *)output_items[0];

  // consume input entities
  consume_each(noutput_items);
  // if we need to find the preamble
  switch (m_state) {
  case FIND_PREAMBLE: {
    GR_LOG_DEBUG(this->d_logger, "DEBUG:Finding preamble");

    // copy input to memory vector for later use.
    // TODO maybe use memcpy for this ?
    for (int i = 0; i < noutput_items; i++) {
      mem_vec.push_back(in[i]);
    }
    // get symbol value of input samples
    //TODO solve problem using the right amount of symbols (currenbtly only the first symb is checked)
    // dechirp the input samples
    volk_32fc_x2_multiply_32fc(&m_dechirped[0], &in[0], &m_downchirp[0], noutput_items);
    // do the fft
    kiss_fft(fft_cfg, (kiss_fft_cpx *)&m_dechirped[0],
             (kiss_fft_cpx *)&cx_out[0]);

    // get abs value of each fft value
    for (int i = 0; i < m_N; i++) {
      m_dfts_mag[i] = std::abs(cx_out[i]);
    }
    m_max_it = std::max_element(m_dfts_mag.begin(), m_dfts_mag.end());
    int test = std::distance(m_dfts_mag.begin(), m_max_it);
//    m_max
//    m_max_it = std::max_element(m_dfts_mag.begin(), m_dfts_mag.end());
//    bin_idx_new = std::distance(m_dfts_mag.begin(), m_max_it);
      bin_idx_new =test;
      // calculate difference between this value and previous value
    if ((bin_idx_new - bin_idx) <= 1) {
      // increase the number of symbols counted
      symbol_cnt++;
    }
    // is symbol value are not close to each other start over
    else {
      // clear memory vector
      mem_vec.clear();
      // set symbol value to be 1
      symbol_cnt = 1;
    }
    // number of preambles needed
    int nR_up = (int)(n_up - 1);
    // if we have n_up-1 symbols counted we have found the preamble
    if (symbol_cnt == nR_up) {
      GR_LOG_DEBUG(this->d_logger, "DEBUG:Found preamble!");
      // set state to be sending packets
      m_state = SEND_FRAMES;
      // dechirp the new potential symbol
      volk_32fc_x2_multiply_32fc(&m_dechirped[(m_fft_symb - 1) * m_N],
                                 &m_input_decim[0], &m_downchirp[0], m_N);
      // set symbol count back to zero
      symbol_cnt = 0;
    }
    return 0;
  }
  case SEND_FRAMES: {
    // actual sending of samples in split packets
    GR_LOG_DEBUG(this->d_logger, "DEBUG:sending packets!");
    // end of vector
    int end_vec = mem_vec.size();
    if (mem_vec.size() > 4 * noutput_items) {
      end_vec = 4 * noutput_items;
    }
    // loop over the output vector and set output to the right values
    for (int i = 0; i < end_vec; i++) {
      out[i] = mem_vec.at(i);
    }
    // clear all vector values that have been used
    mem_vec.erase(mem_vec.begin(), mem_vec.begin() + end_vec);

    // check input on power
    float power = 0;

    // calculate power of input vector
    power = determine_energy(in, (uint32_t)noutput_items);
    // TODO do something meanigfull with current input
    if (power < (float)m_threshold) {
      // current input does not matter we can continue sending frames
      // check if vector is empty
      if (mem_vec.empty()) {
        // if vector is empty go to finding the preamble
        m_state = FIND_PREAMBLE;
        return end_vec;
      }
      // if not empty return number of input items
      return end_vec;
    } else {
      // current input is a LoRa frame we need to do something with this
      for (int i = 0; i < noutput_items; i++) {
        gr_complex temp = in[i];
        mem_vec.push_back(temp);
      }
      return end_vec;
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
