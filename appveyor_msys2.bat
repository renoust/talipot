rem original script taken here: https://github.com/mypaint/libmypaint/blob/master/appveyor.bat
rem Matrix-driven Appveyor CI script for Talipot using GCC compiler provided by MSYS2
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

rem Check which type of Talipot build to perform based on the job number
rem   - odd number = core build
rem   - even number = complete build
rem We build Talipot in two phases (core build then complete build)
rem to avoid AppVeyor build timeouts. Object files generated during
rem the core build will be made available for the complete build
rem thanks to the use of ccache.
set /a TALIPOT_BUILD_CORE_ONLY = %APPVEYOR_JOB_NUMBER% %% 2
echo TALIPOT_BUILD_CORE_ONLY=%TALIPOT_BUILD_CORE_ONLY%

rem Set the paths appropriately
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem Upgrade the MSYS2 platform
bash -lc "pacman --noconfirm --sync --refresh --sysupgrade"

rem Install required tools
bash -lc "pacman --noconfirm -S --needed base-devel unzip"

rem Always install latest GCC toolchain in order to detect possible build failures
rem when its version evolves
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-toolchain"

rem Install the relevant native dependencies
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-ccache"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-yajl"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qhull"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cppunit"

set TALIPOT_BUILD_DOC=OFF

if "%TALIPOT_BUILD_CORE_ONLY%" == "0" (
  goto install_complete_talipot_build_dependencies
) else (
  goto talipot_build
)

:install_complete_talipot_build_dependencies
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-freetype"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-glew"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qt5"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-quazip"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qtwebkit"
rem Workaround a MSYS2 packaging issue for Qt5
rem (see https://github.com/msys2/MINGW-packages/issues/5253)
rem Qt 5.12.3
bash -lc "sed -i -e 's/C:\/building\/msys32/C:\/msys64/g' C:/msys64/mingw64/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake || true"
rem Qt 5.12.4
bash -lc "sed -i -e 's/C:\/building\/msys64/C:\/msys64/g' C:/msys64/mingw64/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake || true"
rem Workaround for QtWebkit detection when current MSYS2 package has not yet been rebuilt
rem against latest Qt 5 version and Qt version detection is too strict in QtWebKit CMake modules
bash -lc "sed -i -e 's/5\.12\.3/5\.12\.4/g' C:/msys64/mingw64/lib/cmake/Qt5WebKit/Qt5WebKitConfig.cmake || true"
bash -lc "sed -i -e 's/5\.12\.3/5\.12\.4/g' C:/msys64/mingw64/lib/cmake/Qt5WebKitWidgets/Qt5WebKitWidgetsConfig.cmake || true"
bash -lc "sed -i -e 's/5\.12\.4/5\.13\.1/g' C:/msys64/mingw64/lib/cmake/Qt5WebKit/Qt5WebKitConfig.cmake || true"
bash -lc "sed -i -e 's/5\.12\.4/5\.13\.1/g' C:/msys64/mingw64/lib/cmake/Qt5WebKitWidgets/Qt5WebKitWidgetsConfig.cmake || true"
bash -lc "sed -i -e 's/5\.13\.1/5\.13\.2/g' C:/msys64/mingw64/lib/cmake/Qt5WebKit/Qt5WebKitConfig.cmake || true"
bash -lc "sed -i -e 's/5\.13\.1/5\.13\.2/g' C:/msys64/mingw64/lib/cmake/Qt5WebKitWidgets/Qt5WebKitWidgetsConfig.cmake || true"
set TALIPOT_BUILD_DOC=ON
goto talipot_build


:talipot_build
rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -lc "set pwd"
bash -lc "env"

rem Install Inetc plugin for NSIS
bash -lc "unzip bundlers/win/Inetc.zip -d\"C:\Program Files (x86)\NSIS\""

rem Install sphinx for Python 3
set PATH=%PYTHON3_HOME%;%PYTHON3_HOME%/Scripts;%PATH%
pip install sphinx

rem Build Talipot with Python 3, run its unit tests and package it
bash -lc "mkdir build"
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE=Release -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=%APPVEYOR_BUILD_FOLDER%/build/install -DTALIPOT_BUILD_CORE_ONLY=%TALIPOT_BUILD_CORE_ONLY% -DTALIPOT_BUILD_DOC=%TALIPOT_BUILD_DOC% -DTALIPOT_BUILD_TESTS=ON -DTALIPOT_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON3_HOME%/python.exe .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 install"
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make runTests"
if %errorlevel% neq 0 exit /b %errorlevel%

if "%TALIPOT_BUILD_CORE_ONLY%" == "0" (
  bash -lc "cd build && make bundle"
)

rem Install sphinx for Python 2
set PATH=%PYTHON2_HOME%;%PYTHON2_HOME%/Scripts;%PATH%
pip install sphinx

rem Build Talipot with Python 2, run its unit tests and package it
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DCMAKE_NEED_RESPONSE=ON -DTALIPOT_BUILD_CORE_ONLY=%TALIPOT_BUILD_CORE_ONLY% -DTALIPOT_BUILD_DOC=%TALIPOT_BUILD_DOC% -DTALIPOT_BUILD_TESTS=ON -DTALIPOT_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON2_HOME%/python.exe .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 install"
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make runTests"
if %errorlevel% neq 0 exit /b %errorlevel%

if "%TALIPOT_BUILD_CORE_ONLY%" == "0" (
  bash -lc "cd build && make bundle"
)
