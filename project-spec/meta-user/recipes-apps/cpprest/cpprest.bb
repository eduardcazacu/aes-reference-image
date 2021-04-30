#
# This file is the cpprest recipe.
#

inherit cmake

SUMMARY = "Microsoft project for cloud-based client-server communication in native code using a modern asynchronous C++ API design."
SECTION = "PETALINUX/apps"
HOMEPAGE = "https://github.com/Microsoft/cpprestsdk/"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${S}/../license.txt;md5=a2e15b954769218ff912468eecd6a02f"
DEPENDS = "openssl zlib boost"

SRC_URI = "git://github.com/Microsoft/cpprestsdk.git;protocol=https;branch=master;tag=v2.10.2"

SRC_URI += "file://fix-cmake-install.patch"

S = "${WORKDIR}/git/Release"

EXTRA_OECMAKE += "-DCPPREST_EXCLUDE_WEBSOCKETS=1"

# EXTRA_OECMAKE += "-DCMAKE_BUILD_TYPE=Release"

inherit cmake
