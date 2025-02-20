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
/* BINDTOOL_HEADER_FILE(partial_ml.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(d45d4840e4e63fe82d7a4d0f50114f14)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <lora_sdr/partial_ml.h>
// pydoc.h is automatically generated in the build directory
#include <partial_ml_pydoc.h>

void bind_partial_ml(py::module& m)
{

    using partial_ml    = ::gr::lora_sdr::partial_ml;


    py::class_<partial_ml, gr::block, gr::basic_block,
        std::shared_ptr<partial_ml>>(m, "partial_ml", D(partial_ml))

        .def(py::init(&partial_ml::make),
           py::arg("sf"),
           py::arg("id"),
           D(partial_ml,make)
        )
        



        ;




}








