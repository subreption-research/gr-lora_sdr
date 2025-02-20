/**
 * @file frame_detector_threshold_impl.h
 * @author Martyn van Dijke (martijnvdijke600@gmail.com)
 * @brief
 * @version 0.3
 * @date 2021-03-23
 *
 *
 */
#ifndef INCLUDED_LORA_SDR_FRAME_DETECTOR_THRESHOLD_IMPL_H
#define INCLUDED_LORA_SDR_FRAME_DETECTOR_THRESHOLD_IMPL_H
#include <gnuradio/io_signature.h>
#include <lora_sdr/frame_detector_threshold.h>
#include <fstream>
#include <iostream>
#include <math.h>
extern "C" {
#include "kiss_fft.h"
}
#define GRLORA_DEBUGV
#define GRLORA_SIM
// #define GRLORA_FILE
namespace gr {
namespace lora_sdr {

class frame_detector_threshold_impl : public frame_detector_threshold {
private:
  /**
   * @brief State the frame finder can be in
   * - FIND_PREAMLBE : find the preamble
   * - SEND_PREAMBLE : send the buffered preamble symbols
   * - SEND_FRAME : send frame
   * - SEND_END_FRAME : send the last part of the frame
   *
   */
  enum State { FIND_PREAMBLE, SEND_PREAMBLE, SEND_FRAME, SEND_END_FRAME };

  /**
   * @brief Current state of the frame finder
   *
   */
  uint8_t m_state;

  /**
   * @brief Spreading factor
   *
   */
  uint8_t m_sf;

  /**
   * @brief Number of samples per LoRa symbol
   *
   */
  uint32_t m_samples_per_symbol;

  /**
   * @brief 2^sf
   *
   */
  uint32_t m_N;

  /**
   * @brief detection threshold
   *
   */
  float m_threshold;

  /**
   * @brief the reference downchirp
   *
   */
  std::vector<gr_complex> m_downchirp;

  /**
   * @brief the dechirped symbols on which we need to perform the FFT.
   *
   */
  std::vector<gr_complex> m_dechirped;

  /**
   * @brief the output of the FFT
   *
   */
  std::vector<gr_complex> cx_out;

  /**
   * @brief the configuration of the FFT
   *
   */
  kiss_fft_cfg fft_cfg;

  /**
   * @briefiterator used to find max and argmax of FFT
   *
   */
  std::vector<float>::iterator m_max_it;

  /**
   * @brief vector containing the magnitude of the FFT.
   *
   */
  std::vector<float> m_dfts_mag;

  /**
   * @brief value of previous lora demodulated symbol
   *
   */
  int32_t bin_idx;

  /**
   * @brief value of newly demodulated symbol
   *
   */
  int32_t bin_idx_new;

  /**
   * @brief Number of consecutive upchirps in preamble
   *
   */
  uint32_t n_up;

  /**
   * @brief Temporary memory vector
   *
   */
  std::vector<gr_complex> buffer;

  /**
   * @brief  LoRa symbol count
   *
   */
  uint16_t symbol_cnt;

  /**
   * @brief Power of a detected LoRa preamble to compare against
   *
   */
  float m_power;

  /**
   * @brief Counter for counting if we are past the net identifier and
   * downchirps once we have found the preamble
   *
   */
  int m_cnt;

  /**
   * @brief boolean variables to tell if we are still in a LoRa frame or not
   *
   */
  bool in_frame;

  /**
   * @brief length in samples of zero append to each frame
   *
   */
  int m_inter_frame_padding;

  /**
   * @brief Counter variable to tell how many extra padding symbols we have
   * processed
   *
   */
  int cnt_padding;

  /**
   * @brief lora symbols per second
   *
   */
  double m_symbols_per_second;

  /**
   * @brief Transmission sampling rate
   *
   */
  uint32_t m_samp_rate;

  /**
   * @brief Transmission bandwidth (Works only for samp_rate=bw)
   *
   */
  uint32_t m_bw;

  /**
   * @brief Temporary gr_complex vector for processing the input per
   * m_samples_processed
   *
   */
  std::vector<gr_complex> m_temp;

  std::vector<float> avg_ratio;

  int m_n_avg;

#ifdef GRLORA_SIM
  /**
   * @brief Tags vector to hold gnuradio simulation tags
   * 
   */
  std::vector <tag_t> m_tags_vector;

  /**
   * @brief Boolean to tell if the beginning of pakcet tag has been detected
   * 
   */
  bool m_detected_tag_begin;

  /**
   * @brief Boolean to tell if the end of pakcet tag has been detected
   * 
   */
  bool m_detected_tag_end;

  /**
   * @brief Variable to hold the packet offset of the end tag
   * 
   */
  u_int32_t m_end_offset;

    /**
   * @brief Variable to hold the packet offset of the end tag
   *
   */
   u_int32_t m_begin_offset;

    u_int32_t m_begin_store;

#endif

#ifdef GRLORA_FILE
    std::ofstream file_ratio;
#endif

  /**
   * @brief Get the symbol object value (aka decoded LoRa symbol value)
   * Function consumes vectors of length m_N
   *
   * @param input : complex samples
   * @return int32_t : LoRa symbol value
   */
  int32_t get_symbol_val(const gr_complex *input);

  /**
   * @brief Checks if current samples have the right
   *
   * @param input : complex input samples
   * @return true : we are in a LoRa frame
   * @return false : we are not in a LoRa frame
   */
  bool check_in_frame(const gr_complex *input);

  /**
   * @brief Calculates the LoRa frame peak power
   *
   * @param input : complex input samples
   * @return float : peak power
   */
  float calc_power(const gr_complex *input);

  /**
   * @brief Set the current LoRa symbol energy
   * Function uses vectors of length m_N
   *
   * @param input : complex samples
   */
  void set_power(const gr_complex *input);

public:
  /**
   * @brief Construct a new frame detector impl object
   *
   * @param samp_rate : sampling rate
   * @param bandwidth : bandwith
   * @param sf : spreading factor
   * @param threshold : threshold value to use
   */
  frame_detector_threshold_impl(uint8_t sf, uint32_t samp_rate, uint32_t bw,
                      float threshold);

  /**
   * @brief Destroy the frame detector impl object
   *
   */
  ~frame_detector_threshold_impl();

  /**
   * @brief
   *
   * @param noutput_items : number of output items
   * @param ninput_items_required : required input items (how many items must we
   * have for we can do something)
   */
  void forecast(int noutput_items, gr_vector_int &ninput_items_required);

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
  int general_work(int noutput_items, gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items);
};

} // namespace lora_sdr
} // namespace gr

#endif /* INCLUDED_LORA_SDR_FRAME_DETECTOR_THRESHOLD_IMPL_H */