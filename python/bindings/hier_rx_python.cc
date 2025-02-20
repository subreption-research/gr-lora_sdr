/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(hier_rx.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(eb7b0607b25f4ba867f5c9e84954a5f8)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <lora_sdr/hier_rx.h>
// pydoc.h is automatically generated in the build directory
#include <hier_rx_pydoc.h>

void bind_hier_rx(py::module& m)
{

    using hier_rx    = ::gr::lora_sdr::hier_rx;


    py::class_<hier_rx, gr::hier_block2,
        std::shared_ptr<hier_rx>>(m, "hier_rx", D(hier_rx))

        .def(py::init(&hier_rx::make),
           py::arg("samp_rate"),
           py::arg("bandwidth"),
           py::arg("sf"),
           py::arg("impl_head"),
           py::arg("cr"),
           py::arg("pay_len"),
           py::arg("has_crc"),
           py::arg("sync_words"),
           py::arg("exit"),
           D(hier_rx,make)
        )
        



        ;




}








