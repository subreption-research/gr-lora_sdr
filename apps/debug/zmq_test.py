#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: cpp_debug
# GNU Radio version: 3.8.2.0

from gnuradio import blocks
from gnuradio import filter
from gnuradio.filter import firdes
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import lora_sdr


class zmq_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "cpp_debug")

        ##################################################
        # Variables
        ##################################################
        self.time_wait = time_wait = 200
        self.threshold = threshold = 100
        self.sf = sf = 9
        self.samp_rate = samp_rate = 250000
        self.pay_len = pay_len = 64
        self.noise = noise = 5
        self.n_frame = n_frame = 5
        self.multi_control = multi_control = True
        self.mult_const = mult_const = 1
        self.impl_head = impl_head = True
        self.has_crc = has_crc = False
        self.frame_period = frame_period = 200
        self.cr = cr = 4
        self.bw = bw = 250000

        ##################################################
        # Blocks
        ##################################################
        self.lora_sdr_hier_tx_1 = lora_sdr.hier_tx(pay_len, n_frame, "TrccpfQHyKfvXswsA4ySxtTiIvi10nSJCUJPYonkWqDHH005UmNfGuocPw3FHKc9", cr, sf, impl_head,has_crc, samp_rate, bw, time_wait, [8, 16],True)
        self.lora_sdr_hier_tx_1.set_min_output_buffer(10000000)
        self.lora_sdr_hier_rx_1 = lora_sdr.hier_rx(samp_rate, bw, sf, impl_head, cr, pay_len, has_crc, [8, 16] , True)
        self.lora_sdr_frame_detector_1 = lora_sdr.frame_detector(sf,threshold)
        self.lora_sdr_frame_detector_1.set_min_output_buffer(20000)
        self.interp_fir_filter_xxx_0_1_0 = filter.interp_fir_filter_ccf(4, (-0.128616616593872,	-0.212206590789194,	-0.180063263231421,	3.89817183251938e-17	,0.300105438719035	,0.636619772367581	,0.900316316157106,	1	,0.900316316157106,	0.636619772367581,	0.300105438719035,	3.89817183251938e-17,	-0.180063263231421,	-0.212206590789194,	-0.128616616593872))
        self.interp_fir_filter_xxx_0_1_0.declare_sample_delay(0)
        self.interp_fir_filter_xxx_0_1_0.set_min_output_buffer(20000)
        self.blocks_throttle_0_1_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate*10,True)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_throttle_0_1_0, 0), (self.lora_sdr_frame_detector_1, 0))
        self.connect((self.interp_fir_filter_xxx_0_1_0, 0), (self.lora_sdr_hier_rx_1, 0))
        self.connect((self.lora_sdr_frame_detector_1, 0), (self.interp_fir_filter_xxx_0_1_0, 0))
        self.connect((self.lora_sdr_hier_tx_1, 0), (self.blocks_throttle_0_1_0, 0))


    def get_time_wait(self):
        return self.time_wait

    def set_time_wait(self, time_wait):
        self.time_wait = time_wait

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold

    def get_sf(self):
        return self.sf

    def set_sf(self, sf):
        self.sf = sf

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0_1_0.set_sample_rate(self.samp_rate*10)

    def get_pay_len(self):
        return self.pay_len

    def set_pay_len(self, pay_len):
        self.pay_len = pay_len

    def get_noise(self):
        return self.noise

    def set_noise(self, noise):
        self.noise = noise

    def get_n_frame(self):
        return self.n_frame

    def set_n_frame(self, n_frame):
        self.n_frame = n_frame

    def get_multi_control(self):
        return self.multi_control

    def set_multi_control(self, multi_control):
        self.multi_control = multi_control

    def get_mult_const(self):
        return self.mult_const

    def set_mult_const(self, mult_const):
        self.mult_const = mult_const

    def get_impl_head(self):
        return self.impl_head

    def set_impl_head(self, impl_head):
        self.impl_head = impl_head

    def get_has_crc(self):
        return self.has_crc

    def set_has_crc(self, has_crc):
        self.has_crc = has_crc

    def get_frame_period(self):
        return self.frame_period

    def set_frame_period(self, frame_period):
        self.frame_period = frame_period

    def get_cr(self):
        return self.cr

    def set_cr(self, cr):
        self.cr = cr

    def get_bw(self):
        return self.bw

    def set_bw(self, bw):
        self.bw = bw





def main(top_block_cls=zmq_test, options=None):
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
