/********************
GNU Radio C++ Flow Graph Source File

Title: cpp_debug
GNU Radio version: 3.8.2.0
********************/

#include "zmq_test.hpp"
using namespace gr;


zmq_test::zmq_test () {



    this->tb = gr::make_top_block("cpp_debug");


// Blocks:
    {
        std::vector<uint16_t> sync_words = {8, 16};
        this->lora_sdr_hier_tx_1 = lora_sdr::hier_tx::make(pay_len, n_frame, "TrccpfQHyKfvXswsA4ySxtTiIvi10nSJCUJPYonkWqDHH005UmNfGuocPw3FHKc9",
                            cr, sf, impl_head,has_crc,
                            samp_rate, bw, time_wait,sync_words,false);
    }
    {
        std::vector<uint16_t> sync_words = {8, 16};
        this->lora_sdr_hier_rx_1 = lora_sdr::hier_rx::make(samp_rate, bw, sf, impl_head, cr, pay_len, has_crc, sync_words ,false);
    }
    {
        this->lora_sdr_frame_detector_1 = lora_sdr::frame_detector::make(sf,samp_rate,bw,threshold);
    }
    {
        std::vector<float> taps = {-0.128616616593872,	-0.212206590789194,	-0.180063263231421,	3.89817183251938e-17	,0.300105438719035	,0.636619772367581	,0.900316316157106,	1	,0.900316316157106,	0.636619772367581,	0.300105438719035,	3.89817183251938e-17,	-0.180063263231421,	-0.212206590789194,	-0.128616616593872};
        this->interp_fir_filter_xxx_0_1_0_0 = filter::interp_fir_filter_ccf::make(4, taps);
    }
    {
        this->blocks_throttle_0_1_0 = blocks::throttle::make(sizeof(gr_complex)*1, samp_rate*10, true);
    }

// Connections:
    this->tb->hier_block2::connect(this->blocks_throttle_0_1_0, 0, this->lora_sdr_frame_detector_1, 0);
    this->tb->hier_block2::connect(this->interp_fir_filter_xxx_0_1_0_0, 0, this->lora_sdr_hier_rx_1, 0);
    this->tb->hier_block2::connect(this->lora_sdr_frame_detector_1, 0, this->interp_fir_filter_xxx_0_1_0_0, 0);
    this->tb->hier_block2::connect(this->lora_sdr_hier_tx_1, 0, this->blocks_throttle_0_1_0, 0);
}

zmq_test::~zmq_test () {
}

// Callbacks:
int zmq_test::get_time_wait () const {
    return this->time_wait;
}

void zmq_test::set_time_wait (int time_wait) {
    this->time_wait = time_wait;
}

int zmq_test::get_threshold () const {
    return this->threshold;
}

void zmq_test::set_threshold (int threshold) {
    this->threshold = threshold;
}

int zmq_test::get_sf () const {
    return this->sf;
}

void zmq_test::set_sf (int sf) {
    this->sf = sf;
}

int zmq_test::get_samp_rate () const {
    return this->samp_rate;
}

void zmq_test::set_samp_rate (int samp_rate) {
    this->samp_rate = samp_rate;
    this->blocks_throttle_0_1_0->set_sample_rate(this->samp_rate*10);
}

int zmq_test::get_pay_len () const {
    return this->pay_len;
}

void zmq_test::set_pay_len (int pay_len) {
    this->pay_len = pay_len;
}

int zmq_test::get_noise () const {
    return this->noise;
}

void zmq_test::set_noise (int noise) {
    this->noise = noise;
}

int zmq_test::get_n_frame () const {
    return this->n_frame;
}

void zmq_test::set_n_frame (int n_frame) {
    this->n_frame = n_frame;
}

bool zmq_test::get_multi_control () const {
    return this->multi_control;
}

void zmq_test::set_multi_control (bool multi_control) {
    this->multi_control = multi_control;
}

int zmq_test::get_mult_const () const {
    return this->mult_const;
}

void zmq_test::set_mult_const (int mult_const) {
    this->mult_const = mult_const;
}

bool zmq_test::get_impl_head () const {
    return this->impl_head;
}

void zmq_test::set_impl_head (bool impl_head) {
    this->impl_head = impl_head;
}

bool zmq_test::get_has_crc () const {
    return this->has_crc;
}

void zmq_test::set_has_crc (bool has_crc) {
    this->has_crc = has_crc;
}

int zmq_test::get_frame_period () const {
    return this->frame_period;
}

void zmq_test::set_frame_period (int frame_period) {
    this->frame_period = frame_period;
}

int zmq_test::get_cr () const {
    return this->cr;
}

void zmq_test::set_cr (int cr) {
    this->cr = cr;
}

int zmq_test::get_bw () const {
    return this->bw;
}

void zmq_test::set_bw (int bw) {
    this->bw = bw;
}


int main (int argc, char **argv) {

    zmq_test* top_block = new zmq_test();
    top_block->tb->start();
    top_block->tb->wait();

    return 0;
}
