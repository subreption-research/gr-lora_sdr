#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "gray_decode_impl.h"
#include "helpers.h"

namespace gr {
  namespace lora_sdr {

    gray_decode::sptr
    gray_decode::make(uint8_t sf)
    {
      return gnuradio::get_initial_sptr
        (new gray_decode_impl(sf));
    }

/**
 * @brief Construct a new gray decode impl::gray decode impl object
 * 
 * @param sf 
 */
    gray_decode_impl::gray_decode_impl(uint8_t sf)
      : gr::sync_block("gray_decode",
              gr::io_signature::make(1, 1, sizeof(uint32_t)),
              gr::io_signature::make(1, 1, sizeof(uint32_t)))
    {
        m_sf=sf;
        set_tag_propagation_policy(TPP_ONE_TO_ONE);
        gr::block::set_thread_priority(94);
    }

/**
 * @brief Destroy the gray decode impl::gray decode impl object
 * 
 */
    gray_decode_impl::~gray_decode_impl()
    {}

/**
 * @brief 
 * 
 * @param noutput_items 
 * @param input_items 
 * @param output_items 
 * @return int 
 */
    int
    gray_decode_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const uint32_t *in = (const uint32_t *) input_items[0];
      uint32_t *out = (uint32_t *) output_items[0];
      for(int i=0;i<noutput_items;i++){
        #ifdef GRLORA_DEBUGV
        std::cout<<std::hex<<"0x"<<in[i]<<" -->  ";
        #endif
        out[i]=in[i];
        for(int j=1;j<m_sf;j++){
             out[i]=out[i]^(in[i]>>j);
        }
        //do the shift of 1
         out[i]=mod(out[i]+1,(1<<m_sf));
         #ifdef GRLORA_DEBUGV
         std::cout<<"0x"<<out[i]<<std::dec<<std::endl;
         #endif
      }

      return noutput_items;
    }
  } /* namespace lora */
} /* namespace gr */
