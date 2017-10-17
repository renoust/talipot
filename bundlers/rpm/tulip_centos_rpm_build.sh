#!/bin/bash

# clean packages cache
yum -y clean all
yum -y clean expire-cache

# install base build system
yum -y install epel-release
yum -y install rpm-build gcc gcc-c++ cmake git subversion tar gzip make ccache

# install tulip deps
yum -y install zlib-devel yajl-devel qhull-devel
yum -y install freetype-devel libpng-devel libjpeg-devel glew-devel
yum -y install qt-devel qtwebkit-devel quazip-devel binutils-devel
yum -y install python python-devel python-pip

# needed for generating the doc
yum -y install doxygen graphviz python-sphinx

# install a more recent version of sphinx
# as sphinx drops python 2.6 support (version installed in CentOS 6)
# in its 1.5 release, use latest version from the 1.4 branch
pip install sphinx==1.4.9

# generate a tulip source tarball with the tulip.spec file in it
cd && mkdir tulip_build && cd tulip_build
cmake -DCMAKE_BUILD_TYPE=Release /tulip
make package_source

# ccache direct mode can not be used with ccache 3.1 (CentOS 6 version)
# when builind rpm packages due to an unsupported compiler option
# so disable it order to get the caching feature functionnal on all CentOS versions
export CCACHE_NODIRECT=1

# build tulip rpm packages
rpmbuild -tb tulip-*.tar.gz

