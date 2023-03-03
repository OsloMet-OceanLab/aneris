#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j)
{
	return i + j;
}

PYBIND11_MODULE(modul, m)
{
	m.doc() = "Test";

	m.def("add", &add, "Add two numbers", py::arg("i"), py::arg("j"));
}
