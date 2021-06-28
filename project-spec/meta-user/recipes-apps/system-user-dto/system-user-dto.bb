#
# This file is the system-user-dto recipe.
#

SUMMARY = "Simple system-user-dto application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
			file://system-user-audio-z7-20.dtbo \
			file://system-user-default-z7-20.dtbo \
			file://system-user-audio-z7-10.dtbo \
			file://system-user-default-z7-10.dtbo \
		  "

S = "${WORKDIR}"

do_install(){
		 install -d ${D}/lib/firmware/top_level_wrapper_z7-20/
		 install -d ${D}/lib/firmware/miniproject_z7-20/
		 install -d ${D}/lib/firmware/top_level_wrapper_z7-10/
		 install -d ${D}/lib/firmware/miniproject_z7-10/

    	 install -m 0644 system-user-default-z7-20.dtbo ${D}/lib/firmware/top_level_wrapper_z7-20/
		 install -m 0644 system-user-audio-z7-20.dtbo ${D}/lib/firmware/miniproject_z7-20/
		 install -m 0644 system-user-default-z7-10.dtbo ${D}/lib/firmware/top_level_wrapper_z7-10/
		 install -m 0644 system-user-audio-z7-10.dtbo ${D}/lib/firmware/miniproject_z7-10/
}

FILES_${PN} += "/lib/firmware/"
FILES_${PN} += "/lib/firmware/miniproject_z7-20/"
FILES_${PN} += "/lib/firmware/top_level_wrapper_z7-20/"
FILES_${PN} += "/lib/firmware/miniproject_z7-10/"
FILES_${PN} += "/lib/firmware/top_level_wrapper_z7-10/"

FILES_${PN} += "/lib/firmware/miniproject_z7-20/system-user-audio-z7-20.dtbo"
FILES_${PN} += "/lib/firmware/top_level_wrapper_z7-20/system-user-default-z7-20.dtbo"
FILES_${PN} += "/lib/firmware/miniproject_z7-10/system-user-audio-z7-10.dtbo"
FILES_${PN} += "/lib/firmware/top_level_wrapper_z7-10/system-user-default-z7-10.dtbo"