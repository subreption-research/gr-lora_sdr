/*
 * Copyright 2021 Free Software Foundation, Inc.
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
/* BINDTOOL_HEADER_FILE(gray_decode.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(5b0ac4b26f2680e61a1b157473c7e82d)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <lora_sdr/gray_decode.h>
// pydoc.h is automatically generated in the build directory
#include <gray_decode_pydoc.h>

void bind_gray_decode(py::module& m)
{

    using gray_decode    = gr::lora_sdr::gray_decode;


    py::class_<gray_decode,
        std::shared_ptr<gray_decode>>(m, "gray_decode", D(gray_decode))

        .def(py::init(&gray_decode::make),
           py::arg("sf"),
           D(gray_decode,make)
        )
        



        ;




}








