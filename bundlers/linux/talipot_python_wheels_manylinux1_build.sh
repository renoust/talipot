#!/bin/bash

# This script is only intended to be run in a pypa/manylinux1
# docker image (based on Centos 5.11)
TALIPOT_PYTHON_TEST_WHEEL_SUFFIX=$1

# install talipot-core wheel deps
yum -y install zlib-devel qhull-devel ccache

pushd /tmp

# download, build and install cmake-2.8.12 needed by talipot
curl -LO https://cmake.org/files/v2.8/cmake-2.8.12.tar.gz
tar zxvf cmake-2.8.12.tar.gz
pushd cmake-2.8.12
cmake28 . && make -j4 install
popd
# remove cmake source
rm -rf cmake-2.8.12*
popd

# get talipot source dir
if [ -d /talipot ]
then
  TALIPOT_SRC=/talipot
else
# talipot sources install
  cd /tmp
  git clone https://github.com/anlambert/talipot.git
  TALIPOT_SRC=/tmp/talipot
fi

# build talipot
if [ -d /talipot_build ]
then
  cd /talipot_build; rm -rf *;
else
  mkdir /tmp/talipot_build; cd /tmp/talipot_build
fi
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  # configure and build python wheel with specific Python version
  cmake ${TALIPOT_SRC} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/talipot_install -DPYTHON_EXECUTABLE=${CPYBIN}/python -DTALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTALIPOT_PYTHON_TEST_WHEEL_SUFFIX=${TALIPOT_PYTHON_TEST_WHEEL_SUFFIX} -DTALIPOT_USE_CCACHE=ON
  make -j4
  make test-wheel
  if [ $? -ne 0 ]
  then
     break
  fi
  # check the test wheel
  pushd ./library/talipot-python/bindings/talipot-core/talipot_module/dist
  ${CPYBIN}/pip install $(ls *${TALIPOT_PYTHON_TEST_WHEEL_SUFFIX}*.whl -t | head -1)
  ${CPYBIN}/python -c "import talipot
from platform import python_version
str = 'talipot successfully imported in Python ' + python_version()
print(str)"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
  # uninstall test wheel
  ${CPYBIN}/pip uninstall -y talipot-python

  # check the talipot-core wheel
  pushd ./library/talipot-python/bindings/talipot-core/talipot_module/dist
  ${CPYBIN}/pip install $(ls -t | head -1)
  ${CPYBIN}/python -c "from talipot import tlp; from platform import python_version; str = 'Talipot ' + tlp.getRelease() + ' successfully imported in Python ' + python_version(); print(str)"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
done
