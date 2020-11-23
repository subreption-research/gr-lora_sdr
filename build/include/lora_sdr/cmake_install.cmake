# Install script for directory: /home/martyn/gr-lora_sdr/include/lora_sdr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/lora_sdr" TYPE FILE FILES
    "/home/martyn/gr-lora_sdr/include/lora_sdr/api.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/lora_sdr.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/add_crc.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/crc_verif.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/dewhitening.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/gray_decode.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/gray_enc.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/hamming_dec.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/hamming_enc.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/header_decoder.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/header.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/interleaver.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/modulate.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/whitening.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/RH_RF95_header.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/fft_demod.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/data_source.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/frame_sync.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/deinterleaver.h"
    "/home/martyn/gr-lora_sdr/include/lora_sdr/err_measures.h"
    )
endif()

