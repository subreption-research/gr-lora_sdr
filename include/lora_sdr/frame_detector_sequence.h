/**
 * @file frame_detector_sequence.h
 * @author Martyn van Dijke (martijnvdijke600@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-14
 * 
 * 
 */
#ifndef INCLUDED_LORA_SDR_FRAME_DETECTOR_SEQUENCE_H
#define INCLUDED_LORA_SDR_FRAME_DETECTOR_SEQUENCE_H

#include <lora_sdr/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace lora_sdr {

      /*!
       * \brief LoRa frame detector sequence, this block detects a LoRa frames using a preamble detection to find the start of the frame
       * and a sequence detection to find the end of the packet.
       * If a frame is detected the frame is outputted to the output.
       *
       * \ingroup lora_sdr
       *
       */
    class LORA_SDR_API frame_detector_sequence : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<frame_detector_sequence> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lora_sdr::frame_detector_sequence.
       *
       * To avoid accidental use of raw pointers, lora_sdr::frame_detector_sequence's
       * constructor is in a private implementation
       * class. lora_sdr::frame_detector_sequence::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint8_t sf, uint32_t samp_rate, uint32_t bw, uint8_t n_seq);
    };

  } // namespace lora_sdr
} // namespace gr

#endif /* INCLUDED_LORA_SDR_FRAME_DETECTOR_SEQUENCE_H */

