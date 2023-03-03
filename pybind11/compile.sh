#!/bin/bash
g++ -O3 -Wall -shared -fPIC $(python3 -m pybind11 --includes) modul.cpp -o modul$(python3-config --extension-suffix)
#g++ -O3 -Wall -shared -fPIC example.cpp -o example$(python3-config --extension-suffix) -I/usr/include/python3.10 -I/home/giorgio/.local/lib/python3.10/site-packages/pybind11/include
