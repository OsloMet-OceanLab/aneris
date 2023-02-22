# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:14:30 2023

@author: OceanLab
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read, write
from IPython.display import Audio
from numpy.fft import fft, ifft

Fs, data = read('C:\\Users\\OceanLab\\Onedrive - OsloMet\\Skrivebord\\test2.wav')

#data = data[:,0] # make audio mono, not needed here

print(f'Sampling frequency is {Fs}')

Audio(data, rate=Fs)

plt.figure()
plt.plot(data)
plt.xlabel('Sample index')
plt.ylabel('Amplitude')
plt.title('Waveform')
plt.show()
