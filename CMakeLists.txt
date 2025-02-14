CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# Remove CMake warnings related to Qt5
IF(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(POLICY CMP0020)

# Needed by clang compiler provided by MacPorts or homebrew
IF(POLICY CMP0025)
  CMAKE_POLICY(SET CMP0025 NEW)
ENDIF(POLICY CMP0025)

# Remove CMake warnings related to rpath on MacOS
IF(POLICY CMP0042)
  CMAKE_POLICY(SET CMP0042 NEW)
ENDIF(POLICY CMP0042)

# Ensure CMAKE_BUILD_WITH_INSTALL_RPATH is still honored with CMake >= 3.9
# This is needed in order to build talipot-python wheels on MacOS
IF(POLICY CMP0068)
  CMAKE_POLICY(SET CMP0068 OLD)
ENDIF(POLICY CMP0068)

IF(POLICY CMP0072)
  CMAKE_POLICY(SET CMP0072 NEW)
ENDIF(POLICY CMP0072)

# enable the use of ccache for Talipot developers to speed up the build process
IF(NOT MSVC)
  SET(TALIPOT_USE_CCACHE OFF CACHE BOOL "Do you want to use ccache to speed up the build process during Talipot development ? [OFF|ON]")
  IF(TALIPOT_USE_CCACHE)
    FIND_PROGRAM(CCACHE_PROGRAM ccache)
    IF(CCACHE_PROGRAM)
      SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    ENDIF(CCACHE_PROGRAM)
  ENDIF(TALIPOT_USE_CCACHE)
ENDIF(NOT MSVC)

PROJECT(talipot)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

#convert CMAKE_INSTALL_PREFIX path into a cmake style path with unix /
#this is to avoid a path with a '\' as the last character which may be seen as
#an escape character (e.g. \")
FILE(TO_CMAKE_PATH "${CMAKE_INSTALL_PREFIX}" CMAKE_INSTALL_PREFIX)

# Force build type to release with debug info if none has been provided
IF(NOT MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
  SET(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
ENDIF(NOT MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "")

## ========================================================
## Check processor architecture
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_SIZEOF_VOID_P}" "8" X86_64)
IF(X86_64)
  SET(PROCESSOR_ARCHITECTURE "64")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DX86_64")
ELSE(X86_64)
  SET(PROCESSOR_ARCHITECTURE "32")
ENDIF(X86_64)

## ========================================================
## Consider *BSD as Linux
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" LINUX)
IF(NOT LINUX)
  STRING(FIND "${CMAKE_SYSTEM_NAME}" "BSD" BSD_POS)
  IF(BSD_POS GREATER -1)
    SET(LINUX TRUE)
    SET(BSD TRUE)
  ENDIF()
ENDIF(NOT LINUX)

## ========================================================
## Operating system preprocessor macros
## ========================================================
IF(LINUX)
  ADD_DEFINITIONS("-D_LINUX")
ENDIF(LINUX)
IF(WIN32)
  ADD_DEFINITIONS("-D_WIN32")
  # ensure WIN32 is defined (as it is not the case when compiling with MinGW and C++11 standard activated)
  ADD_DEFINITIONS("-DWIN32")
  # ensure math defines (e.g. M_PI) are available (as they have been dropped from C++11 standard)
  ADD_DEFINITIONS("-D_USE_MATH_DEFINES")
ENDIF(WIN32)
IF(APPLE)
  ADD_DEFINITIONS("-D__APPLE__")
ENDIF(APPLE)

## ========================================================
## AppImage build
## ========================================================
IF (TALIPOT_BUILD_FOR_APPIMAGE)
  ADD_DEFINITIONS("-DAPPIMAGE_BUILD")
ENDIF (TALIPOT_BUILD_FOR_APPIMAGE)

## ========================================================
## Install paths
## ========================================================

INCLUDE(GNUInstallDirs)

# workaround an install issue on Debian when using CMake < 3
# see https://github.com/Kitware/CMake/commit/620939e4e6f5a61cd5c0fac2704de4bfda0eb7ef
IF(LINUX AND CMAKE_VERSION VERSION_LESS 3.0)
  IF(EXISTS "/etc/debian_version" AND NOT "${CMAKE_INSTALL_PREFIX}" MATCHES "^/usr/?$")
    SET(CMAKE_INSTALL_LIBDIR lib)
  ENDIF(EXISTS "/etc/debian_version" AND NOT "${CMAKE_INSTALL_PREFIX}" MATCHES "^/usr/?$")
ENDIF(LINUX AND CMAKE_VERSION VERSION_LESS 3.0)

SET(TalipotLibInstallDir ${CMAKE_INSTALL_LIBDIR})
SET(TalipotPluginsInstallDir ${TalipotLibInstallDir}/talipot)
SET(TalipotGlyphPluginsInstallDir ${TalipotPluginsInstallDir}/glyph)
SET(TalipotViewPluginsInstallDir ${TalipotPluginsInstallDir}/view)
SET(TalipotInteractorPluginsInstallDir ${TalipotPluginsInstallDir}/interactor)
SET(TalipotBinInstallDir ${CMAKE_INSTALL_BINDIR})
SET(TalipotIncludeInstallDir ${CMAKE_INSTALL_INCLUDEDIR})
SET(TalipotShareInstallDir ${CMAKE_INSTALL_DATADIR}/talipot)
SET(TalipotBitmapInstallDir ${TalipotShareInstallDir}/bitmaps)

## ========================================================
## Talipot CMake macros
## ========================================================
INCLUDE(cmake/TalipotUseFile.cmake)

## ========================================================
## Talipot version variables
## ========================================================
SET(TalipotMajorVersion 1)
SET(TalipotMinorVersion 0)
SET(TalipotReleaseVersion "0-dev")
SET(TalipotVersionSeparator .)
IF(MSVC) #specific options for Visual Studio
  SET(TalipotVersionSeparator _)
ENDIF()
SET(TalipotVersion "${TalipotMajorVersion}${TalipotVersionSeparator}${TalipotMinorVersion}${TalipotVersionSeparator}${TalipotReleaseVersion}")
SET(TalipotMMVersion "${TalipotMajorVersion}${TalipotVersionSeparator}${TalipotMinorVersion}")
SET(TalipotMinorVersionTwoDigits "${TalipotMinorVersion}")
# Use two digits for the minor version when generating the Talipot integer version number
# as there was a 4.10 release that breaks preprocessor test like TALIPOT_INT_VERSION >= 4100 since the 5.0 release
# (see https://sourceforge.net/p/auber/bugs/869/)
STRING(LENGTH "${TalipotMinorVersion}" TalipotMinorVersionNbDigits)
IF(TalipotMinorVersionNbDigits LESS 2)
  SET(TalipotMinorVersionTwoDigits "0${TalipotMinorVersion}")
ENDIF(TalipotMinorVersionNbDigits LESS 2)
SET(TalipotIntMMVersion "${TalipotMajorVersion}${TalipotMinorVersionTwoDigits}")
IF(TalipotReleaseVersion MATCHES "^[0-9]+$")
  SET(TalipotIntVersion "${TalipotMajorVersion}${TalipotMinorVersionTwoDigits}${TalipotReleaseVersion}")
ELSE(TalipotReleaseVersion MATCHES "^[0-9]+$")
  # just a hack to force the display of the full Talipot version in window title bar
  # when it contains non digit characters (like alpha, beta, dev, ...)
  SET(TalipotIntVersion "${TalipotMajorVersion}${TalipotMinorVersion}1")
  INCLUDE(cmake/GetGitRevisionDescription.cmake)
  IF(EXISTS ${PROJECT_SOURCE_DIR}/.git AND NOT TALIPOT_BUILD_FOR_APPIMAGE)
    SET(TALIPOT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
    GET_GIT_HEAD_REVISION(TALIPOT_REFSPEC TALIPOT_GIT_REVISION)
    CONFIGURE_FILE(utils/cmake-templates/GIT_COMMIT.in ${CMAKE_CURRENT_BINARY_DIR}/GIT_COMMIT)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/GIT_COMMIT
            DESTINATION ${TalipotShareInstallDir})
  ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/.git AND NOT TALIPOT_BUILD_FOR_APPIMAGE)
ENDIF(TalipotReleaseVersion MATCHES "^[0-9]+$")

## ========================================================
## Talipot libraries names
## ========================================================
SET(LibTalipotCoreName "talipot-core-${TalipotMMVersion}")
SET(LibTalipotOGLName "talipot-ogl-${TalipotMMVersion}")
SET(LibTalipotGUIName "talipot-gui-${TalipotMMVersion}")
SET(LibTalipotPythonName "talipot-python-${TalipotMMVersion}")
SET(LibTalipotOGDFName "talipot-ogdf-${TalipotMMVersion}")
SET(LibStlPythonBindingsName "stl-python-bindings")
SET(LibTalipotCorePythonBindingsName "talipot-core-python-bindings")
SET(LibTalipotGUIPythonBindingsName "talipot-gui-python-bindings")
SET(TalipotLibs ${LibTalipotCoreName} ${LibTalipotOGLName} ${LibTalipotGUIName} ${LibTalipotOGDFName})

## ========================================================
## Current year (used in documentation config files)
## ========================================================
STRING(TIMESTAMP CurrentYear "%Y")

## ========================================================
## Compiler-specific options
## ========================================================

TALIPOT_SET_COMPILER_OPTIONS()

## ========================================================
## CORE ONLY
## this used to have a minimal build of Talipot without
## any of the opengl, qt or python components
## ========================================================
IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/cmake/FindQtX.cmake OR EMSCRIPTEN)
  SET(TALIPOT_BUILD_CORE_ONLY ON)
  MESSAGE("You can only configure a core build of Talipot.")
ELSE()
  SET(TALIPOT_BUILD_CORE_ONLY OFF CACHE BOOL "Do you want a minimal build of Talipot (no OpenGL rendering, no Qt gui, no documentation) ? [ON|OFF]")
ENDIF()

SET(TALIPOT_PYTHON_ROOT_FOLDER ${CMAKE_BINARY_DIR}/library/talipot-python/bindings/talipot-core/talipot_module)
SET(TALIPOT_PYTHON_FOLDER ${TALIPOT_PYTHON_ROOT_FOLDER}/talipot)
SET(TALIPOTGUI_PYTHON_ROOT_FOLDER ${CMAKE_BINARY_DIR}/library/talipot-python/bindings/talipot-gui/talipotgui_module)
SET(TALIPOTGUI_PYTHON_FOLDER ${TALIPOTGUI_PYTHON_ROOT_FOLDER}/talipotgui)
SET(TALIPOT_PYTHON_NATIVE_FOLDER ${TALIPOT_PYTHON_FOLDER}/native)
SET(TALIPOTGUI_PYTHON_NATIVE_FOLDER ${TALIPOTGUI_PYTHON_FOLDER}/native)
SET(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET OFF CACHE BOOL "Add a wheel make target for generating Python talipot-core module wheel ? [ON|OFF]")
IF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  SET(TALIPOT_BUILD_CORE_ONLY ON)
  IF(APPLE)
    # build binaries with install rpath when generating Talipot Python wheels
    # for MacOs as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  ELSEIF(LINUX)
    FILE(READ /etc/issue LINUX_ISSUE)
    IF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT EXISTS /usr/local/bin/auditwheel)
      MESSAGE(FATAL_ERROR "Python binary wheels for Linux platform must be compiled using the CentOS 5.11 docker image provided by the manylinux project from the Python Packaging Authority.\nSee https://github.com/pypa/manylinux for more details.")
    ENDIF(NOT "${LINUX_ISSUE}" MATCHES ".*CentOS release 5\\.11 \\(Final\\).*" OR NOT EXISTS /usr/local/bin/auditwheel)
    # build binaries with install rpath when generating Talipot Python wheels for Linux
    # as we don't install the project in that case
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  ENDIF(APPLE)
ENDIF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)

## ========================================================
## Required packages
## ========================================================
IF(MINGW)
  # Set some common libraries and headers search paths for MinGW
  SET(CMAKE_LIBRARY_PATH "${MINGW_BIN_PATH};${MINGW_LIB_PATH};${MINGW_LIB64_PATH};${CMAKE_LIBRARY_PATH}")
  SET(CMAKE_INCLUDE_PATH "${MINGW_INCLUDE_PATH};${MINGW_INCLUDE_PATH}/freetype2;${CMAKE_INCLUDE_PATH}")

  # Check if libintl is present in the MinGW build environment
  FIND_LIBRARY(INTL_LIBRARY intl PATHS ${MINGW_LIB_PATH} ${MINGW_LIB64_PATH} NO_DEFAULT_PATH)
ELSE(MINGW)
  FIND_LIBRARY(INTL_LIBRARY intl)
ENDIF(MINGW)
MARK_AS_ADVANCED(INTL_LIBRARY)

# Don't need to find zlib when compiling with emscripten as there is an available emscripten port of it
IF(NOT EMSCRIPTEN)

  # ZLIB
  FIND_PACKAGE(ZLIB REQUIRED)

ENDIF(NOT EMSCRIPTEN)

# Qhull
FIND_PACKAGE(Qhull)

#Yajl
FIND_PACKAGE(Yajl)

IF(NOT TALIPOT_BUILD_CORE_ONLY)
  # OpenGL
  FIND_PACKAGE(OpenGL REQUIRED)

  # Glew
  FIND_PACKAGE(GLEW REQUIRED)
  # if needed, when using cmake 3.15, define some undefined glew variables
  IF(NOT ${CMAKE_VERSION} VERSION_LESS "3.15.0")
    IF(NOT GLEW_INCLUDE_DIR)
      GET_TARGET_PROPERTY(GLEW_INCLUDE_DIR GLEW::GLEW INTERFACE_INCLUDE_DIRECTORIES)
    ENDIF(NOT GLEW_INCLUDE_DIR)
    IF(NOT GLEW_LIBRARY)
      SET(GLEW_LIBRARY GLEW::GLEW)
    ENDIF(NOT GLEW_LIBRARY)
  ENDIF(NOT ${CMAKE_VERSION} VERSION_LESS "3.15.0")

  # Freetype
  FIND_PACKAGE(Freetype REQUIRED)

  # Qt
  FIND_PACKAGE(QtX)

  # Quazip
  FIND_PACKAGE(QuaZip)

ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

# Python
INCLUDE(cmake/TalipotPython.cmake)

## ========================================================
## Include and library paths
## ========================================================
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(GZStreamLibrary gzstream-talipot-${TalipotMMVersion})
IF(NOT TALIPOT_BUILD_CORE_ONLY)
  SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
  SET(FTGLLibrary ftgl-talipot-${TalipotMMVersion})
  SET(Tess2Include ${PROJECT_SOURCE_DIR}/thirdparty/libtess2/Include)
  SET(Tess2Library tess2-talipot-${TalipotMMVersion})
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF/include)
SET(OGDFLibrary OGDF-talipot-${TalipotMMVersion})

SET(VPSCInclude ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libvpsc)
SET(VPSCLibrary vpsc)

SET(UTF8CppInclude ${PROJECT_SOURCE_DIR}/thirdparty/utf8-cpp/source)
SET(StbInclude ${PROJECT_SOURCE_DIR}/thirdparty/stb)

SET(TalipotCoreInclude ${PROJECT_SOURCE_DIR}/library/talipot-core/include/)
SET(TalipotCoreBuildInclude ${PROJECT_BINARY_DIR}/library/talipot-core/include/)
SET(TalipotOGLInclude ${PROJECT_SOURCE_DIR}/library/talipot-ogl/include/)
SET(TalipotOGDFInclude ${PROJECT_SOURCE_DIR}/library/talipot-ogdf/)
SET(TalipotGUIInclude ${PROJECT_SOURCE_DIR}/library/talipot-gui/include/)
SET(TalipotUIGUIInclude ${PROJECT_BINARY_DIR}/library/talipot-gui/src/)
SET(TalipotGUIBuildInclude ${PROJECT_BINARY_DIR}/library/talipot-gui/include/)
SET(TalipotPythonInclude ${PROJECT_SOURCE_DIR}/library/talipot-python/include)

## ========================================================
## Point RPATH to lib folder
## ========================================================
# On MacOS, use rpaths (i.e. the "install_name" field of shared libraries will be @rpath)
# This is already enabled by default for CMake >= 3.0 (see http://www.cmake.org/cmake/help/v3.0/prop_tgt/MACOSX_RPATH.html)
SET(CMAKE_MACOSX_RPATH ON)
FUNCTION(ADD_LIBRARY name)
  _ADD_LIBRARY(${name} ${ARGN})
  IF(APPLE)
    # relative locations of Talipot dylibs dependencies are not the sames when generating Python wheel
    IF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/;@loader_path/../")
    ELSE(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../Frameworks;${QT_FRAMEWORKS_DIR}")
    ENDIF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  ELSE(APPLE)
    # CMake >= 3.1.0 does not allow to set the INSTALL_RPATH property on INTERFACE_LIBRARY target type
    GET_TARGET_PROPERTY(TARGET_TYPE ${name} TYPE)
    IF(NOT "${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN:$ORIGIN/../:$ORIGIN/../../")
      IF(MSVC)
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}")
        SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
        # MSVC linker PDB files are only generated for shared libraries
        IF("${ARGV1}" STREQUAL "SHARED")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_DEBUG "${name}")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_RELWITHDEBINFO "${name}")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
          SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
          INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TalipotShareInstallDir}/pdb CONFIGURATIONS Debug)
          INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TalipotShareInstallDir}/pdb CONFIGURATIONS RelWithDebInfo)
        ENDIF("${ARGV1}" STREQUAL "SHARED")
      ENDIF(MSVC)
    ENDIF(NOT "${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
  ENDIF(APPLE)
  IF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
    ADD_DEPENDENCIES(wheel ${name})
  ENDIF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
ENDFUNCTION()

FUNCTION(ADD_EXECUTABLE name)
  _ADD_EXECUTABLE(${name} ${ARGN})
  IF(APPLE)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR};@loader_path/../Frameworks;${QT_FRAMEWORKS_DIR}")
  ELSEIF(LINUX)
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
  ELSE()
    SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_RPATH "$ORIGIN")
  ENDIF()
  IF(MSVC)
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_DEBUG "${name}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_NAME_RELWITHDEBINFO "${name}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${name} PROPERTIES PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}")
    INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TalipotShareInstallDir}/pdb CONFIGURATIONS Debug)
    INSTALL(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${TalipotShareInstallDir}/pdb CONFIGURATIONS RelWithDebInfo)
  ENDIF(MSVC)
ENDFUNCTION()

## ========================================================
## Tests
## ========================================================
IF(EXISTS ${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
  SET(TALIPOT_BUILD_TESTS OFF CACHE BOOL "Do you want to build the tests ? [OFF|ON]")
ELSE()
  SET(TALIPOT_BUILD_TESTS OFF)
ENDIF()

## ========================================================
## Create library/talipot/include/talipot/Release.h base on Release.h.in
## ========================================================
STRING(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "Release" result1)
STRING(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "RelWithDebInfo" result2)
IF(${result1} OR ${result2})
  SET(TalipotBuildIsRelease 1)
ELSE()
  SET(TalipotBuildIsRelease 0)
ENDIF()
CONFIGURE_FILE("library/talipot-core/include/talipot/Release.h.in" "library/talipot-core/include/talipot/Release.h")

## ========================================================
## Configure file for uninstall target
## ========================================================

IF(LINUX)
  IF("${CMAKE_INSTALL_PREFIX}" MATCHES "^/home.*$")
    SET(DESKTOP_CONFIG_INSTALL_PREFIX "$ENV{HOME}/.local")
  ELSE("${CMAKE_INSTALL_PREFIX}" MATCHES "^/home.*$")
    SET(DESKTOP_CONFIG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  ENDIF("${CMAKE_INSTALL_PREFIX}" MATCHES "^/home.*$")
ENDIF(LINUX)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake VERBATIM)

ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(plugins)
IF(NOT TALIPOT_BUILD_CORE_ONLY)
  ADD_SUBDIRECTORY(doc)
  ADD_SUBDIRECTORY(demos)
  ADD_SUBDIRECTORY(software)
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

IF(TALIPOT_BUILD_TESTS)
  INCLUDE(CTest)
  ENABLE_TESTING()
  IF(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure --build-config "$<CONFIGURATION>")
  ELSE(CMAKE_CONFIGURATION_TYPES)
    ADD_CUSTOM_TARGET(runTests COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure)
  ENDIF(CMAKE_CONFIGURATION_TYPES)
  ADD_SUBDIRECTORY(tests)
ENDIF(TALIPOT_BUILD_TESTS)

INSTALL(FILES AUTHORS
              LICENSE
        DESTINATION ${TalipotShareInstallDir})

INSTALL(FILES cmake/FindTalipot.cmake
        RENAME TalipotConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Talipot/)

CONFIGURE_FILE(cmake/TalipotConfigVersion.cmake.in ${CMAKE_BINARY_DIR}/cmake/TalipotConfigVersion.cmake @ONLY)

INSTALL(FILES cmake/TalipotUseFile.cmake
              ${CMAKE_BINARY_DIR}/cmake/TalipotConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Talipot/)

## ========================================================
## talipot-config script
## ========================================================
SET(prefix "${CMAKE_INSTALL_PREFIX}")
SET(exec_prefix "\${prefix}")
SET(includedir "\${prefix}/${TalipotIncludeInstallDir}")
SET(libdir "\${prefix}/${TalipotLibInstallDir}")
SET(libversion "${TalipotMMVersion}")
SET(VERSION "${TalipotVersion}")
SET(WIN_VERSION "${TalipotMajorVersion}.${TalipotMinorVersion}")
SET(QT_CPPFLAGS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")
IF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
  SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
ENDIF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
IF(WIN32)
  SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY}")
ELSE(WIN32)
  SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY}")
ENDIF(WIN32)
SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE} ${QT_QTWEBKIT_LIBRARY_RELEASE}")
IF(APPLE AND NOT "${CMAKE_OSX_SYSROOT}" STREQUAL "")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot ${CMAKE_OSX_SYSROOT}")
ENDIF(APPLE AND NOT "${CMAKE_OSX_SYSROOT}" STREQUAL "")

CONFIGURE_FILE("talipot-config.in" "talipot-config" @ONLY)
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/talipot-config DESTINATION ${TalipotBinInstallDir})

IF(NOT TALIPOT_BUILD_CORE_ONLY)

  ADD_SUBDIRECTORY(bundlers)
  ADD_SUBDIRECTORY(deploy)

  IF(MSVC)
    INCLUDE(InstallRequiredSystemLibraries)
    IF(OPENMP_FOUND)
      IF(MSVC90)
        INSTALL(PROGRAMS "${MSVC90_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC90.OPENMP/vcomp90.dll" DESTINATION ${TalipotBinInstallDir})
      ENDIF(MSVC90)
      IF(MSVC10)
        INSTALL(PROGRAMS "${MSVC10_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC100.OPENMP/vcomp100.dll" DESTINATION ${TalipotBinInstallDir})
      ENDIF(MSVC10)
      IF(MSVC11)
        INSTALL(PROGRAMS "${MSVC11_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC110.OPENMP/vcomp110.dll" DESTINATION ${TalipotBinInstallDir})
      ENDIF(MSVC11)
      IF(MSVC12)
        INSTALL(PROGRAMS "${MSVC12_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC120.OPENMP/vcomp120.dll" DESTINATION ${TalipotBinInstallDir})
      ENDIF(MSVC12)
      IF(MSVC_VERSION EQUAL 1900)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC140.OPENMP/vcomp140.dll" DESTINATION ${TalipotBinInstallDir})
      # for Visual Studio 15.0 2017
      ELSEIF(MSVC_VERSION EQUAL 1910)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC150.OPENMP/vcomp140.dll" DESTINATION ${TalipotBinInstallDir})
      ELSEIF(MSVC_VERSION GREATER 1910 AND MSVC_VERSION LESS 1920)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC141.OPENMP/vcomp140.dll" DESTINATION ${TalipotBinInstallDir})
      ELSEIF(MSVC_VERSION GREATER_EQUAL 1920)
        INSTALL(PROGRAMS "${MSVC_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC142.OPENMP/vcomp140.dll" DESTINATION ${TalipotBinInstallDir})
      ENDIF()
    ENDIF(OPENMP_FOUND)
  ENDIF(MSVC)

  IF(WIN32)
    # initialize the list of bundle libs (second arg of FIXUP_BUNDLE),
    # updated in plugins/view/GeographicView,
    # and finally used in software/talipot/CMakeLists.txt
    SET_PROPERTY(GLOBAL PROPERTY FIXUP_BUNDLE_LIBS "")

    GET_FILENAME_COMPONENT(NSIS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\NSIS]" ABSOLUTE)
    STRING(COMPARE EQUAL "${NSIS_PATH}" "" NSIS_PATH_EMPTY)
    IF(NSIS_PATH_EMPTY)
      GET_FILENAME_COMPONENT(NSIS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\NSIS]" ABSOLUTE)
      STRING(COMPARE EQUAL "${NSIS_PATH}" "" NSIS_PATH_EMPTY)
    ENDIF(NSIS_PATH_EMPTY)

    IF(NOT NSIS_PATH_EMPTY)

      IF(EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

        STRING(REPLACE "/" "\\" NSIS_PATH "${NSIS_PATH}")
        STRING(REPLACE "/" "\\" BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
        STRING(REPLACE "/" "\\" TLP_DIR "${CMAKE_INSTALL_PREFIX}")
        STRING(REPLACE "/" "\\" QTX_PLUGINS_DIR "${QT_PLUGINS_DIR}")

        IF(TalipotBuildIsRelease OR MSVC)
          SET(DEBUG_MODE "FALSE")
        ELSE(TalipotBuildIsRelease OR MSVC)
          SET(DEBUG_MODE "TRUE")
        ENDIF(TalipotBuildIsRelease OR MSVC)

        IF(MINGW)
          SET(MINGW_BUILD "TRUE")
          SET(BUNDLE_COMMAND cmd //C win_bundle.bat \"${NSIS_PATH}\" \"${TLP_DIR}\" \"${BINARY_DIR}\" ${DEBUG_MODE} ${MINGW_BUILD})
        ELSE(MINGW)
          SET(MINGW_BUILD "FALSE")
          SET(BUNDLE_COMMAND cmd /C "\"win_bundle.bat \"${NSIS_PATH}\" \"${TLP_DIR}\" \"${BINARY_DIR}\" ${DEBUG_MODE} ${MINGW_BUILD}\"")
        ENDIF(MINGW)

        ADD_CUSTOM_TARGET(bundle
                          COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/win_bundle.bat ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/win_bundle.bat
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/FileAssociation.nsh ${CMAKE_CURRENT_BINARY_DIR}/FileAssociation.nsh
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win/LICENSE
                          COMMAND ${BUNDLE_COMMAND}
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/win)

      ELSE(EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

        MESSAGE("NSIS Inetc plugin is required to build the Talipot installer.")
        MESSAGE("Extract ${CMAKE_CURRENT_SOURCE_DIR}/bundlers/win/Inetc.zip in ${NSIS_PATH}.")

      ENDIF(EXISTS "${NSIS_PATH}/Plugins/inetc.dll" OR EXISTS "${NSIS_PATH}/Plugins/x86-ansi/inetc.dll")

    ELSE(NOT NSIS_PATH_EMPTY)

      MESSAGE("Nullsoft Scriptable Install System is not installed on your system")
      MESSAGE("Talipot installer can not be generated without Nullsoft NSIS.")

    ENDIF(NOT NSIS_PATH_EMPTY)

  ENDIF(WIN32)

  IF(APPLE)

    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/mac/mac_bundle.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac/mac_bundle.sh" @ONLY)
    ADD_CUSTOM_TARGET(bundle
                      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
                      COMMAND sh mac_bundle.sh ${CMAKE_CURRENT_BINARY_DIR}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundlers/mac)

  ENDIF(APPLE)

  IF(LINUX)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/bundlers/linux/make_appimage_bundle.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/bundlers/linux/make_appimage_bundle.sh" @ONLY)
  ENDIF(LINUX)

ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)
