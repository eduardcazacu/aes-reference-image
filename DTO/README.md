This directory contains the Device Tree Overlay related files. The files and instructions here are mainly used to build the device tree overlay files for the Audio-Monitor project. 

## DTO kernel config:
follow the kernel config [here](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager).

## DTO Petalinux config:
petalinux-config -DTG Settings -> devicetree flags (-@) \
from  [here](https://forums.xilinx.com/t5/Embedded-Linux/Why-DTBO-fails-on-load-and-says-no-symbols-in-root-of-device/td-p/1088920).

Enable FPGA-MANAGER


## Resources:
https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager \
https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842279/Build+Device+Tree+Blob