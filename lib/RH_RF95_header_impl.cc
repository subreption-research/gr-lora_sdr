#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "RH_RF95_header_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lora_sdr {

RH_RF95_header::sptr RH_RF95_header::make(uint8_t _to, uint8_t _from,
                                          uint8_t _id, uint8_t _flags) {
  return gnuradio::get_initial_sptr(
      new RH_RF95_header_impl(_to, _from, _id, _flags));
}

/**
 * @brief Construct a new rh rf95 header impl::rh rf95 header impl object
 *
 * @param _to
 * @param _from
 * @param _id
 * @param _flags
 */
RH_RF95_header_impl::RH_RF95_header_impl(uint8_t _to, uint8_t _from,
                                         uint8_t _id, uint8_t _flags)
    : gr::block("RH_RF95_header", gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)) {
  m_to = _to;
  m_from = _from;
  m_id = _id;
  m_flags = _flags;

  message_port_register_out(pmt::mp("msg"));
  message_port_register_in(pmt::mp("msg"));
  set_msg_handler(pmt::mp("msg"),
                  [this](pmt::pmt_t msg) { this->msg_handler(msg); });
}

/**
 * @brief Destroy the rh rf95 header impl::rh rf95 header impl object
 *
 */
RH_RF95_header_impl::~RH_RF95_header_impl() {}

/**
 * @brief
 *
 * @param noutput_items
 * @param ninput_items_required
 */
void RH_RF95_header_impl::forecast(int noutput_items,
                                   gr_vector_int &ninput_items_required) {}
void RH_RF95_header_impl::msg_handler(pmt::pmt_t message) {
  std::string str = pmt::symbol_to_string(message);
  std::string s({m_to, m_from, m_id, m_flags});
  str = s + str;
  message_port_pub(pmt::intern("msg"), pmt::mp(str));
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
int RH_RF95_header_impl::general_work(int noutput_items,
                                      gr_vector_int &ninput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items) {
  // Tell runtime system how many output items we produced.
  return 0;
}
} /* namespace lora_sdr */
} /* namespace gr */
