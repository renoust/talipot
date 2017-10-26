#!/bin/bash

# install base system
yum -y install git subversion tar gzip make wget ccache

# add external repo for Qt 4.7
wget http://software.freivald.com/el/5/i386/os/software.freivald.com-2.0.0-0.el.noarch.rpm
rpm -i software.freivald.com-2.0.0-0.el.noarch.rpm

# grab, compile and install CMake 2.8.12
wget --no-check-certificate https://cmake.org/files/v2.8/cmake-2.8.12.2.tar.gz
tar -xvzf cmake-2.8.12.2.tar.gz
cd cmake-2.8.12.2
./bootstrap
make -j4 install

# install tulip deps
yum -y install zlib-devel freetype-devel libpng-devel libjpeg-devel glew-devel
yum -y install qt4-devel

# for CMake to find qmake
export PATH=/usr/lib/qt4/bin:/usr/lib64/qt4/bin:$PATH

# build and install tulip python wheels
mkdir tulip_build && cd tulip_build
cmake -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=/opt/python/cp27-cp27m/bin/python -DTULIP_ACTIVATE_PYTHON_WHEELS_TARGETS=ON -DTULIP_USE_CCACHE=ON /tulip
make -j4 wheels
cmake -DPYTHON_EXECUTABLE=/opt/python/cp27-cp27mu/bin/python .
make -j4 wheels
cmake -DPYTHON_EXECUTABLE=/opt/python/cp27-cp27mu/bin/python .
make -j4 wheels
cmake -DPYTHON_EXECUTABLE=/opt/python/cp33-cp33m/bin/python .
make -j4 wheels
cmake -DPYTHON_EXECUTABLE=/opt/python/cp34-cp34m/bin/python .
make -j4 wheels
cmake -DPYTHON_EXECUTABLE=/opt/python/cp35-cp35m/bin/python .
make -j4 wheels
cmake -DPYTHON_EXECUTABLE=/opt/python/cp36-cp36m/bin/python .
make -j4 wheels
