rem original script taken here: https://github.com/mypaint/libmypaint/blob/master/appveyor.bat
rem Matrix-driven Appveyor CI script for tulip
rem Currently only does MSYS2 builds.
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

rem Set the paths appropriately
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem Upgrade the MSYS2 platform
bash -lc "pacman --noconfirm --sync --refresh --refresh pacman"
bash -lc "pacman --noconfirm --sync --refresh --refresh --sysupgrade --sysupgrade"

rem Install required tools
bash -lc "pacman --noconfirm -S --needed base-devel mingw-w64-%MSYS2_ARCH%-toolchain"

rem Install the relevant native dependencies
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-yajl"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qhull"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-freetype"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-glew"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-libpng"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qt5"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-quazip"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qtwebkit"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-python3"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cppunit"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cmake"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-ccache"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-ninja"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-doxygen"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-python3-sphinx"

rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -lc "set pwd"
bash -lc "env"

bash -lc "ccache -p"

bash -lc "mkdir build"
bash -lc "cd build && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DTULIP_BUILD_TESTS=ON -DTULIP_BUILD_DOC=ON -DTULIP_USE_QT5=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=C:/msys64/%MSYSTEM%/bin/python3.exe .."
bash -lc "cd build && ninja -j4"
bash -lc "cd build && ninja runTests"
