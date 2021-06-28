SUMMARY = "Recipe for  build an external axis-fifo Linux kernel module"
SECTION = "PETALINUX/modules"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit module

SRC_URI = "git://github.com/Tijntj3/Zybo-Z7_Audiomonitor.git;protocol=https;branch=main\
		  "
SRCREV = "${AUTOREV}" 
S = "${WORKDIR}/git/recipes-modules/axisfifo/files"
