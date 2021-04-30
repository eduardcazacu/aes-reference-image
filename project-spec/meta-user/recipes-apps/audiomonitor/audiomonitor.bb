#
# This file is the audiomonitor recipe.
#

SUMMARY = "Simple audiomonitor application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "git://github.com/Tijntj3/Zybo-Z7_Audiomonitor.git;protocol=https;branch=main;tag=v1.0\
		  "

S = "${WORKDIR}/git/Recipe/files"

DEPENDS = "cpprest"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 audiomonitor ${D}${bindir}
	     mkdir -p ${D}/var/www
             cp -R www_Z7-20/* ${D}/var/www
}

FILES_${PN} += "/var/www"
