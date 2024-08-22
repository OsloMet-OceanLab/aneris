# ANERIS bachelorprosjekt
This repository contains all design files and software that were produced during development of Aneris at Oslomet

## Prerequisites
The software was written for a Raspberry Pi 3b+ running Python3.9, however it should be able to run on different versions as well, though this is untested.

## Installation
Installation is relatively simple:

```
sudo make && sudo make install
```

Voilà!

## Running
The software is configured to run at startup as a system service.
It can be stopped by running `sudo systemctl stop aneris`, and disabled from running at startup by running `sudo systemctl disable aneris`

## Controlling the program
The service listens on a unix socket located in `/var/run/aneris.sock`. It can either be interacted with using `aneris-ctl`, or directly by opening the socket.
