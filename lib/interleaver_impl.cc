#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "interleaver_impl.h"
#include <gnuradio/io_signature.h>
#include "helpers.h"

namespace gr {
namespace lora_sdr {

interleaver::sptr interleaver::make(uint8_t cr, uint8_t sf) {
  return gnuradio::get_initial_sptr(new interleaver_impl(cr, sf));
}

/**
 * @brief Construct a new interleaver impl::interleaver impl object
 * 
 * @param cr : coding rate
 * @param sf : spreading factor
 */
interleaver_impl::interleaver_impl(uint8_t cr, uint8_t sf)
    : gr::block("interleaver", gr::io_signature::make(1, 1, sizeof(uint8_t)),
                gr::io_signature::make(1, 1, sizeof(uint32_t))) {
  m_sf = sf;
  m_cr = cr;

  cw_cnt = 0;

  set_tag_propagation_policy(TPP_DONT);
}

/**
 * @brief Destroy the interleaver impl::interleaver impl object
 *
 */
interleaver_impl::~interleaver_impl() {}

/**
 * @brief 
 * 
 * @param noutput_items 
 * @param ninput_items_required 
 */
void interleaver_impl::forecast(int noutput_items,
                                gr_vector_int &ninput_items_required) {
  ninput_items_required[0] = 1;
}

/**
 * @brief 
 * 
 * @param noutput_items 
 * @param ninput_items 
 * @param input_items 
 * @param output_items 
 * @return int 
 */
int interleaver_impl::general_work(int noutput_items,
                                   gr_vector_int &ninput_items,
                                   gr_vector_const_void_star &input_items,
                                   gr_vector_void_star &output_items) {
  const uint8_t *in = (const uint8_t *)input_items[0];
  uint32_t *out = (uint32_t *)output_items[0];
  int nitems_to_process = ninput_items[0];


  //search for work_done tags and if found add them to the stream
  std::vector<tag_t> work_done_tags;
  get_tags_in_window(work_done_tags, 0, 0, ninput_items[0],
                     pmt::string_to_symbol("work_done"));
  if (work_done_tags.size()) {
    add_item_tag(0, nitems_written(0), pmt::intern("work_done"),
                 pmt::intern("done"),pmt::intern("interleaver"));
                 consume_each(ninput_items[0]);
    return 1;
  }

  // read tags
  std::vector<tag_t> tags;
  get_tags_in_window(tags, 0, 0, ninput_items[0],
                     pmt::string_to_symbol("frame_len"));
  if (tags.size()) {
    if (tags[0].offset != nitems_read(0))
      nitems_to_process = tags[0].offset - nitems_read(0);
    else {
      if (tags.size() >= 2) {
        nitems_to_process = tags[1].offset - tags[0].offset;
      }
      cw_cnt = 0;
      m_frame_len = pmt::to_long(tags[0].value);
      tags[0].value = pmt::from_long(
          8 + std::max((int)std::ceil((double)(m_frame_len - m_sf + 2) / m_sf) *
                           (m_cr + 4),
                       0)); // get number of items in frame
      tags[0].offset = nitems_written(0);
    }
  }

  // nitems_to_process = std::min(nitems_to_process)
  // handle the first interleaved block special case
  uint8_t cw_len = 4 + ((cw_cnt < m_sf - 2) ? 4 : m_cr);
  uint8_t sf_app = (cw_cnt < m_sf - 2) ? m_sf - 2 : m_sf;

  nitems_to_process = std::min(nitems_to_process, (int)sf_app);
  if (std::floor((float)noutput_items / cw_len) == 0) {
    return 0;
  }

  if (nitems_to_process >= sf_app ||
      cw_cnt + nitems_to_process == m_frame_len) {
    // propagate tag
    if (!cw_cnt)
      add_item_tag(0, tags[0]);

    // Create the empty matrices
    std::vector<std::vector<bool>> cw_bin(sf_app);
    std::vector<bool> init_bit(m_sf, 0);
    std::vector<std::vector<bool>> inter_bin(cw_len, init_bit);

    // convert to input codewords to binary vector of vector
    for (int i = 0; i < sf_app; i++) {
      if (i >= nitems_to_process) // ninput_items[0])
        cw_bin[i] = int2bool((uint8_t )0, cw_len);
      else
        cw_bin[i] = int2bool(in[i], cw_len);
      cw_cnt++;
    }

#ifdef GRLORA_DEBUGV
    std::cout << "codewords---- " << std::endl;
    for (uint32_t i = 0u; i < sf_app; i++) {
      for (int j = 0; j < int(cw_len); j++) {
        std::cout << cw_bin[i][j];
      }
      std::cout << " 0x" << std::hex << (int)in[i] << std::dec << std::endl;
    }
    std::cout << std::endl;
#endif
    // Do the actual interleaving
    for (int32_t i = 0; i < cw_len; i++) {
      for (int32_t j = 0; j < int(sf_app); j++) {
        inter_bin[i][j] = cw_bin[mod((i - j - 1), sf_app)][i];
      }
      // For the first bloc we add a parity bit and a zero in the end of the
      // lora symbol(reduced rate)
      if (cw_cnt == m_sf - 2)
        inter_bin[i][sf_app] =
            accumulate(inter_bin[i].begin(), inter_bin[i].end(), 0) % 2;

      out[i] = bool2int(inter_bin[i]);
    }

#ifdef GRLORA_DEBUGV    
    std::cout << "interleaved----" << std::endl;
    for (uint32_t i = 0u; i < cw_len; i++) {
      for (int j = 0; j < int(m_sf); j++) {
        std::cout << inter_bin[i][j];
      }
      std::cout << " " << out[i] << std::endl;
    }
    std::cout << std::endl;
#endif
    consume_each(nitems_to_process > sf_app ? sf_app : nitems_to_process);
    return cw_len;
  } else
    return 0;
}

} // namespace lora_sdr
} /* namespace gr */
