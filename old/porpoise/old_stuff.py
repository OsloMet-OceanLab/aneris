# -*- coding: utf-8 -*-
"""
Created on Mon Feb 27 14:55:37 2023

@author: salve
"""


import numpy as np

def signaltonoise(a, axis=0, ddof=0):
    a = np.asanyarray(a)
    m = a.mean(axis)
    sd = a.std(axis=axis, ddof=ddof)
    return np.where(sd == 0, 0, m/sd)

def noise(d):
    data = np.frombuffer(bytearray(d), dtype=np.int16)
    #norm = data / (max(np.amax(data), -1 * np.amin(data)))
    #return signaltonoise(norm)
    data //= 2
    for i, x in np.ndenumerate(data):
        if x < -3000 or x > 7000:
            data[i] = 0
    return data
