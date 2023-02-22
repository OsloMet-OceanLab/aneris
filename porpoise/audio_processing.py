# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:14:30 2023

@author: OceanLab
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read, write
#from IPython.display import Audio
from numpy.fft import fft, ifft

rate, data = read('C:\\Users\\OceanLab\\Onedrive - OsloMet\\Skrivebord\\test2.wav')

#data = data[:,0] # make audio mono, not needed here

print(f'Sampling frequency is {rate}')

#Audio(data, rate=Fs)

plt.figure()
plt.plot(data)
plt.xlabel('Sample index')
plt.ylabel('Amplitude')
plt.title('Waveform')
plt.show()

import noisereduce as nr

reduced_noise = nr.reduce_noise(y=data, sr=rate)

plt.figure()
plt.plot(reduced_noise)
plt.xlabel('Sample index')
plt.ylabel('Amplitude')
plt.title('Waveform (reduced)')
plt.show()

write('C:\\Users\\OceanLab\\Onedrive - OsloMet\\Skrivebord\\test4.wav', rate, reduced_noise)
