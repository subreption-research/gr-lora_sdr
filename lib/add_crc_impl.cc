#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "add_crc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lora_sdr {

add_crc::sptr add_crc::make(bool has_crc) {
  return gnuradio::get_initial_sptr(new add_crc_impl(has_crc));
}

/**
 * @brief Construct a new add crc impl object
 *
 * @param has_crc : boolean if crc is turned on or not
 */
add_crc_impl::add_crc_impl(bool has_crc)
    : gr::block("add_crc", gr::io_signature::make(1, 1, sizeof(uint8_t)),
                gr::io_signature::make(1, 1, sizeof(uint8_t))) {
  m_has_crc = has_crc;
  set_tag_propagation_policy(TPP_DONT);
}

/**
 * @brief Destroy the add crc impl::add crc impl object
 *
 */
add_crc_impl::~add_crc_impl() {}

  /**
   * @brief Standard gnuradio function for telling the scheduler how many input items are needed
   *
   * @param noutput_items number of input items
   * @param ninput_items_required minimum items required
   */
void add_crc_impl::forecast(int noutput_items,
                            gr_vector_int &ninput_items_required) {
  ninput_items_required[0] = 1;
}

/**
  /**
   * @brief Calculates the crc value for a given byte
   *
   * @param crcValue : current crc value
   * @param newByte : byte for calculate the crc value for
   * @return unsigned int
   */
unsigned int add_crc_impl::crc16(unsigned int crcValue, unsigned char newByte) {
  unsigned char i;
  for (i = 0; i < 8; i++) {

    if (((crcValue & 0x8000) >> 8) ^ (newByte & 0x80)) {
      crcValue = (crcValue << 1) ^ 0x1021;
    } else {
      crcValue = (crcValue << 1);
    }
    newByte <<= 1;
  }
  return crcValue;
}

/**
 * @brief stanard gnuradio function that does the actual computations
 *
 * @param noutput_items number of output items
 * @param ninput_items number of input items
 * @param input_items input items (input data)
 * @param output_items output items (output data)
 * @return int
 */
int add_crc_impl::general_work(int noutput_items, gr_vector_int &ninput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items) {
  const uint8_t *in = (const uint8_t *)input_items[0];
  uint8_t *out = (uint8_t *)output_items[0];
  int nitems_to_output = 0;
  noutput_items = std::max(0, noutput_items - 4); // take margin to output CRC
  int nitems_to_process = std::min(ninput_items[0], noutput_items);
    //search for work_done tags and if found add them to the stream
    std::vector<tag_t> work_done_tags;
    get_tags_in_window(work_done_tags, 0, 0, nitems_read(0) + 10,
                       pmt::string_to_symbol("work_done"));
    if (work_done_tags.size()) {
        add_item_tag(0, nitems_written(0), pmt::intern("work_done"),
                     pmt::intern("done"),pmt::intern("add crc"));
                     consume_each(ninput_items[0]);
                     return 1;
    }
  // read tags
  std::vector<tag_t> tags;
  get_tags_in_window(tags, 0, 0, noutput_items,
                     pmt::string_to_symbol("payload_str"));
  if (tags.size()) {

    if (tags[0].offset != nitems_read(0)) {
      nitems_to_process =
          std::min(tags[0].offset - nitems_read(0), (uint64_t)noutput_items);
    } else {
      if (tags.size() >= 2) {
        nitems_to_process =
            std::min(tags[1].offset - tags[0].offset, (uint64_t)noutput_items);
      }
      std::string str = pmt::symbol_to_string(tags[0].value);
      std::copy(str.begin(), str.end(), std::back_inserter(m_payload));
      // pass tags downstream
      get_tags_in_window(tags, 0, 0, ninput_items[0],
                         pmt::string_to_symbol("frame_len"));
      m_frame_len = pmt::to_long(tags[0].value);
      tags[0].offset = nitems_written(0);
      tags[0].value = pmt::from_long(m_frame_len + (m_has_crc ? 4 : 0));

      if (nitems_to_process)
        add_item_tag(0, tags[0]);

      m_cnt = 0;
    }
  }
  if (!nitems_to_process) {
    return 0;
  }
  m_cnt += nitems_to_process;
  if (m_has_crc && m_cnt == m_frame_len &&
      nitems_to_process) { // append the CRC to the payload
    uint16_t crc = 0x0000;
    m_payload_len = m_payload.size();
    // calculate CRC on the N-2 firsts data bytes using Poly=1021 Init=0000
    for (int i = 0; i < (int)m_payload_len - 2; i++)
      crc = crc16(crc, m_payload[i]);

    // XOR the obtained CRC with the last 2 data bytes
    crc = crc ^ m_payload[m_payload_len - 1] ^
          (m_payload[m_payload_len - 2] << 8);
    // Place the CRC in the correct output nibble
    out[nitems_to_process] = ((crc & 0x000F));
    out[nitems_to_process + 1] = ((crc & 0x00F0) >> 4);
    out[nitems_to_process + 2] = ((crc & 0x0F00) >> 8);
    out[nitems_to_process + 3] = ((crc & 0xF000) >> 12);

    nitems_to_output = nitems_to_process + 4;
    m_payload.clear();
  } else {
    nitems_to_output = nitems_to_process;
  }
  memcpy(out, in, nitems_to_process * sizeof(uint8_t));
  consume_each(nitems_to_process);

  return nitems_to_output;
}
} // namespace lora_sdr
} /* namespace gr */
