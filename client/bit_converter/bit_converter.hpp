#ifndef BIT_CONVERTER_HPP
#define BIT_CONVERTER_HPP

#include <cstdint>

#ifdef CPYTHON

#include <pybind11/pybind11.h>
namespace py = pybind11;

#endif

namespace bit_converter
{

#ifdef CPYTHON
py::bytes
#else
char *
#endif
bytes_to_16(const char *buf = nullptr, size_t len = 0, bool invertEndianness = true);

#ifdef CPYTHON
py::bytes
#else
char *
#endif
bytes_to_32(const char *buf = nullptr, size_t len = 0, bool invertEndianness = true);

#ifdef CPYTHON
py::bytes
#else
char *
#endif

invert_endianness_24(const char *buf = nullptr, size_t len = 0);

} // end namespace bit_converter

#ifdef CPYTHON

PYBIND11_MODULE(bit_converter, m)
{
        m.doc() = "Convert a 24 bit signed buffer to either 16 or 32 bit";

        m.def("bytes_to_16", &bit_converter::bytes_to_16, "Convert a 24 bit sample to 16 bit",
                py::arg("buf") = nullptr, py::arg("len") = 0, py::arg("invertEndianness") = true);
        m.def("bytes_to_32", &bit_converter::bytes_to_32, "Convert a 24 bit sample to 32 bit",
                py::arg("buf") = nullptr, py::arg("len") = 0, py::arg("invertEndianness") = true);
        m.def("invert_endianness_24", &bit_converter::invert_endianness_24, "Change the endianness of the sample",
                py::arg("buf") = nullptr, py::arg("len") = 0);
}

#endif

#endif
