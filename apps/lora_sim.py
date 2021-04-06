#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: test
# Author: Martyn
# Description: Simulation example LoRa
# GNU Radio version: 3.8.2.0

from gnuradio import blocks
from gnuradio import channels
from gnuradio.filter import firdes
from gnuradio import filter
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import lora_sdr
import threading


class lora_sim(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "test")

        self._lock = threading.RLock()

        ##################################################
        # Variables
        ##################################################
        self.bw = bw = 250000
        self.sf = sf = 9
        self.samp_rate = samp_rate = bw
        self.pay_len = pay_len = 64
        self.n_frame = n_frame = 3
        self.multi_control = multi_control = True
        self.mult_const = mult_const = 1
        self.mean = mean = 200
        self.impl_head = impl_head = True
        self.has_crc = has_crc = False
        self.frame_period = frame_period = 200
        self.cr = cr = 4

        ##################################################
        # Blocks
        ##################################################
        self.lora_sdr_whitening_0_0 = lora_sdr.whitening()
        self.lora_sdr_modulate_0_0 = lora_sdr.modulate(sf, samp_rate, bw, [8, 16],False)
        self.lora_sdr_modulate_0_0.set_min_output_buffer(10000000)
        self.lora_sdr_interleaver_0_0 = lora_sdr.interleaver(cr, sf)
        self.lora_sdr_header_decoder_0_0 = lora_sdr.header_decoder(impl_head, cr, pay_len, has_crc)
        self.lora_sdr_header_0_0 = lora_sdr.header(impl_head, has_crc, cr)
        self.lora_sdr_hamming_enc_0_0 = lora_sdr.hamming_enc(cr, sf)
        self.lora_sdr_hamming_dec_0_0 = lora_sdr.hamming_dec()
        self.lora_sdr_gray_enc_0_0 = lora_sdr.gray_enc()
        self.lora_sdr_gray_decode_0_0 = lora_sdr.gray_decode(sf)
        self.lora_sdr_frame_sync_0_0 = lora_sdr.frame_sync(samp_rate, bw, sf, impl_head, [8, 16])
        self.lora_sdr_fft_demod_0_0 = lora_sdr.fft_demod(samp_rate, bw, sf, impl_head)
        self.lora_sdr_dewhitening_0_0 = lora_sdr.dewhitening()
        self.lora_sdr_deinterleaver_0_0 = lora_sdr.deinterleaver(sf)
        self.lora_sdr_data_source_sim_0 = lora_sdr.data_source_sim(64, n_frame, '', 200, True)
        self.lora_sdr_crc_verif_0_0 = lora_sdr.crc_verif(False)
        self.lora_sdr_add_crc_0_0 = lora_sdr.add_crc(has_crc)
        self.interp_fir_filter_xxx_0_1 = filter.interp_fir_filter_ccf(4, (-0.128616616593872,	-0.212206590789194,	-0.180063263231421,	3.89817183251938e-17	,0.300105438719035	,0.636619772367581	,0.900316316157106,	1	,0.900316316157106,	0.636619772367581,	0.300105438719035,	3.89817183251938e-17,	-0.180063263231421,	-0.212206590789194,	-0.128616616593872))
        self.interp_fir_filter_xxx_0_1.declare_sample_delay(0)
        self.interp_fir_filter_xxx_0_1.set_min_output_buffer(20000)
        self.channels_channel_model_0_0 = channels.channel_model(
            noise_voltage=0.0,
            frequency_offset=0.0,
            epsilon=1.0,
            taps=[1.0 + 0.0j],
            noise_seed=0,
            block_tags=False)
        self.blocks_throttle_0_1 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate*10,True)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*1)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.lora_sdr_data_source_sim_0, 'msg'), (self.lora_sdr_whitening_0_0, 'msg'))
        self.msg_connect((self.lora_sdr_header_decoder_0_0, 'frame_info'), (self.lora_sdr_frame_sync_0_0, 'frame_info'))
        self.connect((self.blocks_throttle_0_1, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.blocks_throttle_0_1, 0), (self.channels_channel_model_0_0, 0))
        self.connect((self.channels_channel_model_0_0, 0), (self.interp_fir_filter_xxx_0_1, 0))
        self.connect((self.interp_fir_filter_xxx_0_1, 0), (self.lora_sdr_frame_sync_0_0, 0))
        self.connect((self.lora_sdr_add_crc_0_0, 0), (self.lora_sdr_hamming_enc_0_0, 0))
        self.connect((self.lora_sdr_data_source_sim_0, 0), (self.lora_sdr_whitening_0_0, 0))
        self.connect((self.lora_sdr_deinterleaver_0_0, 0), (self.lora_sdr_hamming_dec_0_0, 0))
        self.connect((self.lora_sdr_dewhitening_0_0, 0), (self.lora_sdr_crc_verif_0_0, 0))
        self.connect((self.lora_sdr_fft_demod_0_0, 0), (self.lora_sdr_gray_enc_0_0, 0))
        self.connect((self.lora_sdr_frame_sync_0_0, 0), (self.lora_sdr_fft_demod_0_0, 0))
        self.connect((self.lora_sdr_gray_decode_0_0, 0), (self.lora_sdr_modulate_0_0, 0))
        self.connect((self.lora_sdr_gray_enc_0_0, 0), (self.lora_sdr_deinterleaver_0_0, 0))
        self.connect((self.lora_sdr_hamming_dec_0_0, 0), (self.lora_sdr_header_decoder_0_0, 0))
        self.connect((self.lora_sdr_hamming_enc_0_0, 0), (self.lora_sdr_interleaver_0_0, 0))
        self.connect((self.lora_sdr_header_0_0, 0), (self.lora_sdr_add_crc_0_0, 0))
        self.connect((self.lora_sdr_header_decoder_0_0, 0), (self.lora_sdr_dewhitening_0_0, 0))
        self.connect((self.lora_sdr_interleaver_0_0, 0), (self.lora_sdr_gray_decode_0_0, 0))
        self.connect((self.lora_sdr_modulate_0_0, 0), (self.blocks_throttle_0_1, 0))
        self.connect((self.lora_sdr_whitening_0_0, 0), (self.lora_sdr_header_0_0, 0))


    def get_bw(self):
        return self.bw

    def set_bw(self, bw):
        with self._lock:
            self.bw = bw
            self.set_samp_rate(self.bw)

    def get_sf(self):
        return self.sf

    def set_sf(self, sf):
        with self._lock:
            self.sf = sf

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        with self._lock:
            self.samp_rate = samp_rate
            self.blocks_throttle_0_1.set_sample_rate(self.samp_rate*10)

    def get_pay_len(self):
        return self.pay_len

    def set_pay_len(self, pay_len):
        with self._lock:
            self.pay_len = pay_len

    def get_n_frame(self):
        return self.n_frame

    def set_n_frame(self, n_frame):
        with self._lock:
            self.n_frame = n_frame

    def get_multi_control(self):
        return self.multi_control

    def set_multi_control(self, multi_control):
        with self._lock:
            self.multi_control = multi_control

    def get_mult_const(self):
        return self.mult_const

    def set_mult_const(self, mult_const):
        with self._lock:
            self.mult_const = mult_const

    def get_mean(self):
        return self.mean

    def set_mean(self, mean):
        with self._lock:
            self.mean = mean

    def get_impl_head(self):
        return self.impl_head

    def set_impl_head(self, impl_head):
        with self._lock:
            self.impl_head = impl_head

    def get_has_crc(self):
        return self.has_crc

    def set_has_crc(self, has_crc):
        with self._lock:
            self.has_crc = has_crc

    def get_frame_period(self):
        return self.frame_period

    def set_frame_period(self, frame_period):
        with self._lock:
            self.frame_period = frame_period

    def get_cr(self):
        return self.cr

    def set_cr(self, cr):
        with self._lock:
            self.cr = cr





def main(top_block_cls=lora_sim, options=None):
    tb = top_block_cls()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        sys.exit(0)

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    tb.start()

    tb.wait()


if __name__ == '__main__':
    main()
