# CMake generated Testfile for 
# Source directory: /home/martyn/gr-lora_sdr/python
# Build directory: /home/martyn/gr-lora_sdr/build/python
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(qa_lora_sdr "/usr/bin/sh" "/home/martyn/gr-lora_sdr/build/python/qa_lora_sdr_test.sh")
set_tests_properties(qa_lora_sdr PROPERTIES  _BACKTRACE_TRIPLES "/usr/lib64/cmake/gnuradio/GrTest.cmake;122;add_test;/home/martyn/gr-lora_sdr/python/CMakeLists.txt;45;GR_ADD_TEST;/home/martyn/gr-lora_sdr/python/CMakeLists.txt;0;")
