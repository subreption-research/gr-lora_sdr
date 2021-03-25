#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 "Martyn van Dijke".
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import lora_sdr_swig as lora_sdr
import pmt
import time
import filecmp
import os
from gnuradio import filter
from gnuradio.filter import firdes


class qa_tx_rx(gr_unittest.TestCase):
    """Unit test case class that simulates entire tx and rx sides and checks if input data is the same as output data.
    Will return True if input data == output_data and will return False (i.e. failed test case) if its not the same.

    Args:
        gr_unittest ([type]): gnuradio test case class
    """

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_40(self):
        """
        Test case generated by test-case generator
        """
        ##################################################
        # Variables
        ##################################################
        # Input data into the system
        src_data = "PKdhtXMmr18n2L9K88eMlGn7CcctT9RwKSB1FebW397VI5uG1yhc3uavuaOb9vyJ"
        self.bw = bw = 250000
        self.sf = sf = 9
        self.samp_rate = samp_rate = bw
        self.pay_len = pay_len = 64
        self.n_frame = n_frame = 10
        self.impl_head = impl_head = True
        self.has_crc = has_crc = True
        self.frame_period = frame_period = 200
        self.cr = cr = 4

        ##################################################
        # Blocks
        ##################################################
        self.lora_sdr_hier_tx_0 = lora_sdr.hier_tx(pay_len, n_frame,src_data
                                                   , cr, sf,
                                                   impl_head, has_crc, samp_rate, bw, 200, True)
        self.lora_sdr_hier_rx_0_1_0_0_1_0 = lora_sdr.hier_rx(samp_rate, bw, sf, impl_head, cr, pay_len, has_crc, True)
        self.interp_fir_filter_xxx_0_0 = filter.interp_fir_filter_ccf(4, (
        -0.128616616593872, -0.212206590789194, -0.180063263231421, 3.89817183251938e-17, 0.300105438719035, 0.636619772367581,
        0.900316316157106, 1, 0.900316316157106, 0.636619772367581, 0.300105438719035, 3.89817183251938e-17, -0.180063263231421,
        -0.212206590789194, -0.128616616593872))
        self.interp_fir_filter_xxx_0_0.declare_sample_delay(0)
        self.interp_fir_filter_xxx_0_0.set_min_output_buffer(1024)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex * 1, samp_rate, True)
        #get the output
        self.blocks_message_debug_0 = blocks.message_debug()


        ##################################################
        # Connections
        ##################################################
        self.tb.connect((self.blocks_throttle_0_0, 0), (self.interp_fir_filter_xxx_0_0, 0))
        self.tb.connect((self.interp_fir_filter_xxx_0_0, 0), (self.lora_sdr_hier_rx_0_1_0_0_1_0, 0))
        self.tb.connect((self.lora_sdr_hier_tx_0, 0), (self.blocks_throttle_0_0, 0))
        #output msg connection
        self.tb.msg_connect((self.lora_sdr_hier_rx_0_1_0_0_1_0, 'msg'),
                            (self.blocks_message_debug_0, 'store'))

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
                self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)

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

        # run the flowgraph, since we use a message strobe we have to run and stop the flowgraph with some computation time inbetween
        self.tb.start()
        # time.sleep(10)
        # self.tb.stop()
        self.tb.wait()
        num_messages = self.blocks_message_debug_0.num_messages()
        if num_messages > 1:
            # try to get get the message from the store port of the message debug printer and convert to string from pmt message
            try:
                msg = pmt.symbol_to_string(
                    self.blocks_message_debug_0.get_message(1))
            except:
                # if not possible set message to be None
                msg = None
        else:
            # try to get get the message from the store port of the message debug printer and convert to string from pmt message
            try:
                msg = pmt.symbol_to_string(
                    self.blocks_message_debug_0.get_message(0))
            except:
                # if not possible set message to be None
                msg = None

        # check if message received is the same as the message decoded
        self.assertMultiLineEqual(
            src_data, msg, msg="Error decoded data {0} is not the same as input data {1}".format(msg, src_data))

        # self.assertMultiLineEqual(src_data, msg, msg="Error decoded data {0} is not the same as input data {1}.".format(
        #     msg, src_data


if __name__ == '__main__':
    gr_unittest.run(qa_tx_rx)
