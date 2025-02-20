#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Cran reciever
# Author: Martyn van Dijke
# Description: CRAN reciever
# GNU Radio version: 3.8.2.0

from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
import sys
import lora_sdr
import threading
import pickle
import codecs
class cran_recieve(gr.top_block):

    def __init__(self, flowgraph_vars, pipe):
        gr.top_block.__init__(self, "Cran reciever")
        self._lock = threading.RLock()
        # print(flowgraph_vars)
        ##################################################
        # Variables
        ##################################################
        self.bw = bw = flowgraph_vars['bw']
        self.sf = sf = flowgraph_vars['sf']
        self.samp_rate = samp_rate = flowgraph_vars['samp_rate']
        self.pay_len = pay_len = flowgraph_vars['pay_len']
        self.n_frame = n_frame = 2
        self.multi_control = multi_control = True
        self.impl_head = impl_head = flowgraph_vars['impl_head']
        self.has_crc = has_crc = flowgraph_vars['has_crc']
        self.frame_period = frame_period = 2
        self.cr = cr = flowgraph_vars['cr']

        ##################################################
        # Blocks
        ##################################################
        self.lora_sdr_hier_rx_1 = lora_sdr.hier_rx(samp_rate, bw, sf, impl_head, cr, pay_len, has_crc, [8, 16] , True)
        self.lora_sdr_hier_rx_1.set_min_output_buffer(65536)
        self.lora_sdr_frame_reciever_0 = lora_sdr.frame_reciever(pipe, 5555, 'Rx' ,False)
        self.lora_sdr_frame_reciever_0.set_min_output_buffer(65536)
        self.interp_fir_filter_xxx_0_1_0 = filter.interp_fir_filter_ccf(4, (-0.128616616593872,	-0.212206590789194,	-0.180063263231421,	3.89817183251938e-17	,0.300105438719035	,0.636619772367581	,0.900316316157106,	1	,0.900316316157106,	0.636619772367581,	0.300105438719035,	3.89817183251938e-17,	-0.180063263231421,	-0.212206590789194,	-0.128616616593872))
        self.interp_fir_filter_xxx_0_1_0.declare_sample_delay(0)
        self.interp_fir_filter_xxx_0_1_0.set_min_output_buffer(65536)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_message_debug_0 = blocks.message_debug()



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.lora_sdr_hier_rx_1, 'msg'), (self.blocks_message_debug_0, 'store'))
        self.connect((self.blocks_throttle_0, 0), (self.interp_fir_filter_xxx_0_1_0, 0))
        self.connect((self.interp_fir_filter_xxx_0_1_0, 0), (self.lora_sdr_hier_rx_1, 0))
        self.connect((self.lora_sdr_frame_reciever_0, 0), (self.blocks_throttle_0, 0))


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
            self.blocks_throttle_0.set_sample_rate(self.samp_rate)

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

def main(flowgraph_vars,pipe,top_block_cls=cran_recieve, options=None):
    tb = top_block_cls(flowgraph_vars, pipe)
    tb.start()
    
    while True:
        num_messages = tb.blocks_message_debug_0.num_messages()
        if num_messages >= 1:
            tb.stop()
            exit(0)



if __name__ == '__main__':
    #decode flowgraphs vars
    unpickled = pickle.loads(codecs.decode(sys.argv[1].encode(), "base64"))
    pipe = sys.argv[2]
    # async_mode = False
    # if async_mode:
    #     unpickled = unpickled['flowgraph_vars']
    # input = pickle.load(sys.stdin.buffer)
    main(unpickled, pipe)
