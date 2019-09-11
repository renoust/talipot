#!/bin/bash

# This script is only intended to be run in a pypa/manylinux*
# docker image (based on Centos 5.11 and CentOS 6.10)
TULIP_PYTHON_TEST_WHEEL_SUFFIX=$1

# install tulip-core wheel deps
yum -y install zlib-devel qhull-devel ccache

# ensure python library from based system is present, even if we do
# not link to it, as cmake will fail to find PythonLibs otherwise
yum -y install python-devel

# download, build and install cmake-2.8.12 needed by tulip (only on CentOS 5)
if grep -q "CentOS release 5.11" /etc/issue
then
  pushd /tmp
  curl -LO https://cmake.org/files/v2.8/cmake-2.8.12.tar.gz
  tar zxvf cmake-2.8.12.tar.gz
  pushd cmake-2.8.12
  cmake28 . && make -j4 install
  popd
  # remove cmake source
  rm -rf cmake-2.8.12*
  popd
fi

# get tulip source dir
if [ -d /tulip ]
then
  TULIP_SRC=/tulip
else
# tulip sources install
  cd /tmp
  git clone https://github.com/Tulip-Dev/tulip.git
  TULIP_SRC=/tmp/tulip
fi

cat > ~/.pypirc << EOF
[distutils]
index-servers=
    testpypi
[testpypi]
repository: https://test.pypi.org/legacy/
username: __token__
password: $2
EOF

# build tulip
if [ -d /tulip_build ]
then
  cd /tulip_build; rm -rf *;
else
  mkdir /tmp/tulip_build; cd /tmp/tulip_build
fi
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  ${CPYBIN}/pip install twine
  # configure and build python wheel with specific Python version
  cmake ${TULIP_SRC} -DCMAKE_BUILD_TYPE=Release \
                     -DCMAKE_INSTALL_PREFIX=/tmp/tulip_install \
                     -DPYTHON_EXECUTABLE=${CPYBIN}/python \
                     -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON \
                     -DTULIP_PYTHON_TEST_WHEEL_SUFFIX=${TULIP_PYTHON_TEST_WHEEL_SUFFIX} \
                     -DTULIP_USE_CCACHE=ON
  make -j4
  if [ -n "$TULIP_PYTHON_TEST_WHEEL_SUFFIX" ]
  then
    make test-wheel
    make test-wheel-upload
  else
    make wheel
  fi
  if [ $? -ne 0 ]
  then
     break
  fi

  if [ -n "$TULIP_PYTHON_TEST_WHEEL_SUFFIX" ]
  then
    # check the test wheel
    pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
    ${CPYBIN}/pip install $(ls *${TULIP_PYTHON_TEST_WHEEL_SUFFIX}*.whl -t | head -1)
    ${CPYBIN}/python -c "
from tulip import tlp
from platform import python_version
print('Tulip %s successfully imported in Python %s' %
      (tlp.getTulipRelease(), python_version()))
"
    if [ $? -ne 0 ]
    then
      break
    fi
    popd
    # uninstall test wheel
    ${CPYBIN}/pip uninstall -y tulip-python
  fi

#   # check the tulip-core wheel
#   pushd ./library/tulip-python/bindings/tulip-core/tulip_module/dist
#   ${CPYBIN}/pip install $(ls -t | head -1)
#   ${CPYBIN}/python -c "
# from tulip import tlp
# from platform import python_version
# print('Tulip %s successfully imported in Python %s' %
#       (tlp.getTulipRelease(), python_version()))
# "
#   if [ $? -ne 0 ]
#   then
#      break
#   fi
#   popd
done
