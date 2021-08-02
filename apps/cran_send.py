#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Frame detector test with noise and cfo
# Author: Martyn van Dijke
# Description: Simulation example LoRa
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
import threading


class cran_send(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Frame detector test with noise and cfo")

        self._lock = threading.RLock()

        ##################################################
        # Variables
        ##################################################
        self.bw = bw = 250000
        self.time_wait = time_wait = 200
        self.sto = sto = 0.3
        self.snr = snr = -7
        self.sf = sf = 9
        self.samp_rate = samp_rate = bw
        self.pay_len = pay_len = 64
        self.n_frame = n_frame = 20
        self.multi_control = multi_control = True
        self.impl_head = impl_head = True
        self.has_crc = has_crc = False
        self.frame_period = frame_period = 200
        self.cr = cr = 4
        self.cfo = cfo = 0.2
        self.center_freq = center_freq = 868.1e6

        ##################################################
        # Blocks
        ##################################################
        self.lora_sdr_hier_tx_1 = lora_sdr.hier_tx(pay_len, n_frame, "TrccpfQHyKfvXswsA4ySxtTiIvi10nSJCUJPYonkWqDHH005UmNfGuocPw3FHKc9", cr, sf, impl_head,has_crc, samp_rate, bw, time_wait, [8, 16],False)
        self.lora_sdr_hier_tx_1.set_min_output_buffer(4096)
        self.lora_sdr_hier_rx_0 = lora_sdr.hier_rx(samp_rate, bw, sf, impl_head, cr, pay_len, has_crc, [8, 16] , True)
        self.lora_sdr_frame_sender_0 = lora_sdr.frame_sender('localhost', 5555, True, True, sf, samp_rate, bw, has_crc, pay_len, cr, impl_head, [8, 16])
        self.lora_sdr_frame_detector_timeout_0_0 = lora_sdr.frame_detector_timeout(sf,samp_rate,bw,180,False)
        self.interp_fir_filter_xxx_0_1_0 = filter.interp_fir_filter_ccf(4, (-0.128616616593872,	-0.212206590789194,	-0.180063263231421,	3.89817183251938e-17	,0.300105438719035	,0.636619772367581	,0.900316316157106,	1	,0.900316316157106,	0.636619772367581,	0.300105438719035,	3.89817183251938e-17,	-0.180063263231421,	-0.212206590789194,	-0.128616616593872))
        self.interp_fir_filter_xxx_0_1_0.declare_sample_delay(0)
        self.interp_fir_filter_xxx_0_1_0.set_min_output_buffer(4096)
        self.blocks_throttle_0_1_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate*10,True)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_gr_complex*1, '', "")
        self.blocks_tag_debug_0.set_display(True)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_throttle_0_1_0_0, 0), (self.lora_sdr_frame_detector_timeout_0_0, 0))
        self.connect((self.interp_fir_filter_xxx_0_1_0, 0), (self.lora_sdr_hier_rx_0, 0))
        self.connect((self.lora_sdr_frame_detector_timeout_0_0, 0), (self.blocks_tag_debug_0, 0))
        self.connect((self.lora_sdr_frame_detector_timeout_0_0, 0), (self.interp_fir_filter_xxx_0_1_0, 0))
        self.connect((self.lora_sdr_frame_detector_timeout_0_0, 0), (self.lora_sdr_frame_sender_0, 0))
        self.connect((self.lora_sdr_hier_tx_1, 0), (self.blocks_throttle_0_1_0_0, 0))


    def get_bw(self):
        return self.bw

    def set_bw(self, bw):
        with self._lock:
            self.bw = bw
            self.set_samp_rate(self.bw)

    def get_time_wait(self):
        return self.time_wait

    def set_time_wait(self, time_wait):
        with self._lock:
            self.time_wait = time_wait

    def get_sto(self):
        return self.sto

    def set_sto(self, sto):
        with self._lock:
            self.sto = sto

    def get_snr(self):
        return self.snr

    def set_snr(self, snr):
        with self._lock:
            self.snr = snr

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
            self.blocks_throttle_0_1_0_0.set_sample_rate(self.samp_rate*10)

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

    def get_cfo(self):
        return self.cfo

    def set_cfo(self, cfo):
        with self._lock:
            self.cfo = cfo

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        with self._lock:
            self.center_freq = center_freq





def main(top_block_cls=cran_send, options=None):
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
