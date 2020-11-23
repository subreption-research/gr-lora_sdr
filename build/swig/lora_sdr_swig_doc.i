
/*
 * This file was automatically generated using swig_doc.py.
 *
 * Any changes to it will be lost next time it is regenerated.
 */




%feature("docstring") gr::lora_sdr::add_crc "This block add LoRa CRC (Cyclic redundancy check) into the payload, more info see add_crc_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::add_crc.

To avoid accidental use of raw pointers, lora_sdr::add_crc's constructor is in a private implementation class. lora_sdr::add_crc::make is the public interface for creating new instances.

Args:
    has_crc : "

%feature("docstring") gr::lora_sdr::add_crc::make "This block add LoRa CRC (Cyclic redundancy check) into the payload, more info see add_crc_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::add_crc.

To avoid accidental use of raw pointers, lora_sdr::add_crc's constructor is in a private implementation class. lora_sdr::add_crc::make is the public interface for creating new instances.

Args:
    has_crc : "

%feature("docstring") gr::lora_sdr::crc_verif "Verifies if the included crc is correct or not for more information about the implementation visit crc_verify_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::crc_verif.

To avoid accidental use of raw pointers, lora_sdr::crc_verif's constructor is in a private implementation class. lora_sdr::crc_verif::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::crc_verif::make "Verifies if the included crc is correct or not for more information about the implementation visit crc_verify_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::crc_verif.

To avoid accidental use of raw pointers, lora_sdr::crc_verif's constructor is in a private implementation class. lora_sdr::crc_verif::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::data_source "Data source that can both generate random strings or static strings, for more information about the implementation visit data_source_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::data_source.

To avoid accidental use of raw pointers, lora_sdr::data_source's constructor is in a private implementation class. lora_sdr::data_source::make is the public interface for creating new instances.

Args:
    pay_len : 
    n_frames : "

%feature("docstring") gr::lora_sdr::data_source::make "Data source that can both generate random strings or static strings, for more information about the implementation visit data_source_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::data_source.

To avoid accidental use of raw pointers, lora_sdr::data_source's constructor is in a private implementation class. lora_sdr::data_source::make is the public interface for creating new instances.

Args:
    pay_len : 
    n_frames : "

%feature("docstring") gr::lora_sdr::deinterleaver "Deinterleaves the received payload, for more information about the implementation visit deinterleaver_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::deinterleaver.

To avoid accidental use of raw pointers, lora_sdr::deinterleaver's constructor is in a private implementation class. lora_sdr::deinterleaver::make is the public interface for creating new instances.

Args:
    sf : "

%feature("docstring") gr::lora_sdr::deinterleaver::make "Deinterleaves the received payload, for more information about the implementation visit deinterleaver_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::deinterleaver.

To avoid accidental use of raw pointers, lora_sdr::deinterleaver's constructor is in a private implementation class. lora_sdr::deinterleaver::make is the public interface for creating new instances.

Args:
    sf : "

%feature("docstring") gr::lora_sdr::dewhitening "Dewhites the received payload, for more information about the implementation visit crc_verify_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::dewhitening.

To avoid accidental use of raw pointers, lora_sdr::dewhitening's constructor is in a private implementation class. lora_sdr::dewhitening::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::dewhitening::make "Dewhites the received payload, for more information about the implementation visit crc_verify_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::dewhitening.

To avoid accidental use of raw pointers, lora_sdr::dewhitening's constructor is in a private implementation class. lora_sdr::dewhitening::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::err_measures "Simple block that can count and output the number of Bit Error's (BE) ,for more information about the implementation visit err_measures_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::err_measures.

To avoid accidental use of raw pointers, lora_sdr::err_measures's constructor is in a private implementation class. lora_sdr::err_measures::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::err_measures::make "Simple block that can count and output the number of Bit Error's (BE) ,for more information about the implementation visit err_measures_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::err_measures.

To avoid accidental use of raw pointers, lora_sdr::err_measures's constructor is in a private implementation class. lora_sdr::err_measures::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::fft_demod "FFT demodulation block ,for more information about the implementation visit fft_demod_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::fft_demod.

To avoid accidental use of raw pointers, lora_sdr::fft_demod's constructor is in a private implementation class. lora_sdr::fft_demod::make is the public interface for creating new instances.

Args:
    samp_rate : 
    bandwidth : 
    sf : 
    impl_head : "

%feature("docstring") gr::lora_sdr::fft_demod::make "FFT demodulation block ,for more information about the implementation visit fft_demod_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::fft_demod.

To avoid accidental use of raw pointers, lora_sdr::fft_demod's constructor is in a private implementation class. lora_sdr::fft_demod::make is the public interface for creating new instances.

Args:
    samp_rate : 
    bandwidth : 
    sf : 
    impl_head : "

%feature("docstring") gr::lora_sdr::frame_sync "Block that is able to detect the received spectrum and start to find the data such that it can be decoded. This block is at the heart of the the RX (decoding) side and houses a lot of logic and data manipulation. For more information about the implementation visit frame_sync_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::frame_sync.

To avoid accidental use of raw pointers, lora_sdr::frame_sync's constructor is in a private implementation class. lora_sdr::frame_sync::make is the public interface for creating new instances.

Args:
    samp_rate : 
    bandwidth : 
    sf : 
    impl_head : "

%feature("docstring") gr::lora_sdr::frame_sync::make "Block that is able to detect the received spectrum and start to find the data such that it can be decoded. This block is at the heart of the the RX (decoding) side and houses a lot of logic and data manipulation. For more information about the implementation visit frame_sync_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::frame_sync.

To avoid accidental use of raw pointers, lora_sdr::frame_sync's constructor is in a private implementation class. lora_sdr::frame_sync::make is the public interface for creating new instances.

Args:
    samp_rate : 
    bandwidth : 
    sf : 
    impl_head : "

%feature("docstring") gr::lora_sdr::gray_decode "Decode the received payload with the inverse gray mapping in order to obtain the send message. For more information about the implementation visit gray_decode_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::gray_decode.

To avoid accidental use of raw pointers, lora_sdr::gray_decode's constructor is in a private implementation class. lora_sdr::gray_decode::make is the public interface for creating new instances.

Args:
    sf : "

%feature("docstring") gr::lora_sdr::gray_decode::make "Decode the received payload with the inverse gray mapping in order to obtain the send message. For more information about the implementation visit gray_decode_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::gray_decode.

To avoid accidental use of raw pointers, lora_sdr::gray_decode's constructor is in a private implementation class. lora_sdr::gray_decode::make is the public interface for creating new instances.

Args:
    sf : "

%feature("docstring") gr::lora_sdr::gray_enc "Modulate the payload with an Gray map. This ensures that bits are modulated such that two subsequent symbols differ one bit. For more information about the implementation visit gray_enc_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::gray_enc.

To avoid accidental use of raw pointers, lora_sdr::gray_enc's constructor is in a private implementation class. lora_sdr::gray_enc::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::gray_enc::make "Modulate the payload with an Gray map. This ensures that bits are modulated such that two subsequent symbols differ one bit. For more information about the implementation visit gray_enc_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::gray_enc.

To avoid accidental use of raw pointers, lora_sdr::gray_enc's constructor is in a private implementation class. lora_sdr::gray_enc::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::hamming_dec "Hamming decoding stage, decode the received payload. If necessary this block will preform error detection and correction using the hamming code parity bits. For more information about the implementation visit hamming_dec_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::hamming_dec.

To avoid accidental use of raw pointers, lora_sdr::hamming_dec's constructor is in a private implementation class. lora_sdr::hamming_dec::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::hamming_dec::make "Hamming decoding stage, decode the received payload. If necessary this block will preform error detection and correction using the hamming code parity bits. For more information about the implementation visit hamming_dec_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::hamming_dec.

To avoid accidental use of raw pointers, lora_sdr::hamming_dec's constructor is in a private implementation class. lora_sdr::hamming_dec::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::hamming_enc "Add hamming code to the to be sent playload. This means extra party bits are added to the payload to be able to recover from bit errors during transmission For more information about the implementation visit hamming_enc_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::hamming_enc.

To avoid accidental use of raw pointers, lora_sdr::hamming_enc's constructor is in a private implementation class. lora_sdr::hamming_enc::make is the public interface for creating new instances.

Args:
    cr : 
    sf : "

%feature("docstring") gr::lora_sdr::hamming_enc::make "Add hamming code to the to be sent playload. This means extra party bits are added to the payload to be able to recover from bit errors during transmission For more information about the implementation visit hamming_enc_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::hamming_enc.

To avoid accidental use of raw pointers, lora_sdr::hamming_enc's constructor is in a private implementation class. lora_sdr::hamming_enc::make is the public interface for creating new instances.

Args:
    cr : 
    sf : "

%feature("docstring") gr::lora_sdr::header "Encode the header of the data package For more information about the implementation visit header_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::header.

To avoid accidental use of raw pointers, lora_sdr::header's constructor is in a private implementation class. lora_sdr::header::make is the public interface for creating new instances.

Args:
    impl_head : 
    has_crc : 
    cr : "

%feature("docstring") gr::lora_sdr::header::make "Encode the header of the data package For more information about the implementation visit header_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::header.

To avoid accidental use of raw pointers, lora_sdr::header's constructor is in a private implementation class. lora_sdr::header::make is the public interface for creating new instances.

Args:
    impl_head : 
    has_crc : 
    cr : "

%feature("docstring") gr::lora_sdr::header_decoder "Decode the header of the data spackage. For more information about the implementation visit header_decoder_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::header_decoder.

To avoid accidental use of raw pointers, lora_sdr::header_decoder's constructor is in a private implementation class. lora_sdr::header_decoder::make is the public interface for creating new instances.

Args:
    impl_head : 
    cr : 
    pay_len : 
    has_crc : "

%feature("docstring") gr::lora_sdr::header_decoder::make "Decode the header of the data spackage. For more information about the implementation visit header_decoder_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::header_decoder.

To avoid accidental use of raw pointers, lora_sdr::header_decoder's constructor is in a private implementation class. lora_sdr::header_decoder::make is the public interface for creating new instances.

Args:
    impl_head : 
    cr : 
    pay_len : 
    has_crc : "

%feature("docstring") gr::lora_sdr::interleaver "Interleaves the to be sent data, in order to gain more resilience against bit transmission errors For more information about the implementation visit interleaver_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::interleaver.

To avoid accidental use of raw pointers, lora_sdr::interleaver's constructor is in a private implementation class. lora_sdr::interleaver::make is the public interface for creating new instances.

Args:
    cr : 
    sf : "

%feature("docstring") gr::lora_sdr::interleaver::make "Interleaves the to be sent data, in order to gain more resilience against bit transmission errors For more information about the implementation visit interleaver_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::interleaver.

To avoid accidental use of raw pointers, lora_sdr::interleaver's constructor is in a private implementation class. lora_sdr::interleaver::make is the public interface for creating new instances.

Args:
    cr : 
    sf : "

%feature("docstring") gr::lora_sdr::lora_sdr "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::lora_sdr.

To avoid accidental use of raw pointers, lora_sdr::lora_sdr's constructor is in a private implementation class. lora_sdr::lora_sdr::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::lora_sdr::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::lora_sdr.

To avoid accidental use of raw pointers, lora_sdr::lora_sdr's constructor is in a private implementation class. lora_sdr::lora_sdr::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::modulate "End block of the sending side that modulates the final to be sent data to the LoRa standard For more information about the implementation visit modulate_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::modulate.

To avoid accidental use of raw pointers, lora_sdr::modulate's constructor is in a private implementation class. lora_sdr::modulate::make is the public interface for creating new instances.

Args:
    sf : 
    samp_rate : 
    bw : "

%feature("docstring") gr::lora_sdr::modulate::make "End block of the sending side that modulates the final to be sent data to the LoRa standard For more information about the implementation visit modulate_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::modulate.

To avoid accidental use of raw pointers, lora_sdr::modulate's constructor is in a private implementation class. lora_sdr::modulate::make is the public interface for creating new instances.

Args:
    sf : 
    samp_rate : 
    bw : "

%feature("docstring") gr::lora_sdr::RH_RF95_header "Encode the data in the RH_RF95 header format For more information about the implementation visit RH_RF95_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::RH_RF95_header.

To avoid accidental use of raw pointers, lora_sdr::RH_RF95_header's constructor is in a private implementation class. lora_sdr::RH_RF95_header::make is the public interface for creating new instances.

Args:
    _to : 
    _from : 
    _id : 
    _flags : "

%feature("docstring") gr::lora_sdr::RH_RF95_header::make "Encode the data in the RH_RF95 header format For more information about the implementation visit RH_RF95_impl.

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::RH_RF95_header.

To avoid accidental use of raw pointers, lora_sdr::RH_RF95_header's constructor is in a private implementation class. lora_sdr::RH_RF95_header::make is the public interface for creating new instances.

Args:
    _to : 
    _from : 
    _id : 
    _flags : "

%feature("docstring") gr::lora_sdr::whitening "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::whitening.

To avoid accidental use of raw pointers, lora_sdr::whitening's constructor is in a private implementation class. lora_sdr::whitening::make is the public interface for creating new instances."

%feature("docstring") gr::lora_sdr::whitening::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of lora_sdr::whitening.

To avoid accidental use of raw pointers, lora_sdr::whitening's constructor is in a private implementation class. lora_sdr::whitening::make is the public interface for creating new instances."

%feature("docstring") mod "return the modulus ab between 0 and (b-1)"

%feature("docstring") int2bool "Convert an integer into a MSB first vector of bool."

%feature("docstring") bool2int "Convert a MSB first vector of bool to a integer."

%feature("docstring") build_ref_chirps "Return the reference chirps using s_f=bw."

%feature("docstring") build_upchirp "Return an modulated upchirp using s_f=bw."