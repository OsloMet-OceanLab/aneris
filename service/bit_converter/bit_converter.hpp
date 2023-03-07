#ifndef BIT_CONVERTER_HPP
#define BIT_CONVERTER_HPP

#include <cstdint>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace bit_converter
{

py::bytes numto16(const char *buf = nullptr, size_t len = 0, bool invertEndianness = true);
py::bytes numto32(const char *buf = nullptr, size_t len = 0, bool invertEndianness = true);
py::bytes invertEndianness24(const char *buf = nullptr, size_t len = 0);
py::bytes return_bytes(char *buf = nullptr, size_t len = 0);

} // end namespace bit_converter

PYBIND11_MODULE(bit_converter, m)
{
        m.doc() = "Module to convert a 24 bit signed array to either 16 or 32 bit";

        m.def("numto16", &bit_converter::numto16, "Convert a 24 bit sample to 16 bit",
                py::arg("buf") = nullptr, py::arg("len") = 0, py::arg("invertEndianness") = true);
        m.def("numto32", &bit_converter::numto32, "Convert a 24 bit sample to 32 bit",
                py::arg("buf") = nullptr, py::arg("len") = 0, py::arg("invertEndianness") = true);
        m.def("invertEndianness24", &bit_converter::invertEndianness24, "Change the endianness of the sample",
                py::arg("buf") = nullptr, py::arg("len") = 0);
        m.def("return_bytes", &bit_converter::return_bytes, "Returns supplied bytes",
                py::arg("buf") = nullptr, py::arg("len") = 0);
}

#endif
