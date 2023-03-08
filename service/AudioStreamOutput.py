# -*- coding: utf-8 -*-
"""
Created on Wed Mar  8 13:26:19 2023

@author: salve
"""

from io import BytesIO
from threading import Condition

class AudioStreamOutput:
    def __init__(self):
        self.frame = None
        self.buffer = BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\x0a\x06'):
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()[18:]
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)
