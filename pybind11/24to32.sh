#!/bin/bash
g++ -O3 -Wall -shared -fPIC $(python3 -m pybind11 --includes) bit_converter.cpp -o bit_converter$(python3-config --extension-suffix)
