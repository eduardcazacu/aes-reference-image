This directory contains the Device Tree Overlay related files. The files and instructions here are mainly used to build the device tree overlay files for the Audio-Monitor project. 

## DTO kernel config:
follow the kernel config [here](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager).

## DTO Petalinux config:
petalinux-config -DTG Settings -> devicetree flags (-@) \
from  [here](https://forums.xilinx.com/t5/Embedded-Linux/Why-DTBO-fails-on-load-and-says-no-symbols-in-root-of-device/td-p/1088920).

Enable FPGA-MANAGER

## Generate DTO from .hdf
start hsi:
```bash
hsi
```

then:
```bash
open_hw_design <path to .hdf>
set_repo_path <project-root/device-tree-xlnx>
create_sw_design device-tree -os device_tree -proc ps7_cortexa9_0
set_property CONFIG.dt_overlay true [get_os]
generate_target -dir dts_output/
exit
```

## Compile .dtbo
Use dtc version 1.4.5. Source code is found in /dtc/. Build by calling make in the /dec/ directory

to compile the audio-monitor.dtbo:
```bash
cd audio-monitor/zybo-z7-20/
../../dtc/dtc -O dtb -o system-user.dtbo -b 0 -@ system-user.dtsi
```


## Resources:
https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager \
https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842279/Build+Device+Tree+Blob