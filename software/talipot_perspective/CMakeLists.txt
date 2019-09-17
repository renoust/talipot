SET(SRCS
  src/main.cpp
  src/PerspectiveMainWindow.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_CPP(MOC_SRCS
  include/PerspectiveMainWindow.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude} ${OPENGL_INCLUDE_DIR} include/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(WIN32)

STRING(TIMESTAMP CURRENT_YEAR "%Y")

INCLUDE(generate_product_version)
generate_product_version(
   VersionFiles
   NAME "Talipot Perspective launcher"
   ICON ${PROJECT_SOURCE_DIR}/library/talipot-gui/resources/icons/talipot.ico
   VERSION_MAJOR ${TalipotMajorVersion}
   VERSION_MINOR ${TalipotMinorVersion}
   VERSION_PATCH ${TalipotReleaseVersion}
   COMPANY_NAME "The Talipot developers"
   COMPANY_COPYRIGHT "(C) Copyright ${CURRENT_YEAR}"
   ORIGINAL_FILENAME "talipot_perspective.exe"
)

SET(SRCS ${SRCS} ${VersionFiles})

ENDIF(WIN32)

ADD_EXECUTABLE(talipot_perspective ${SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(talipot_perspective crash_handling ${LibTalipotCoreName} ${LibTalipotOGLName} ${LibTalipotGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS talipot_perspective
  RUNTIME DESTINATION ${TalipotBinInstallDir} COMPONENT talipot_app
  LIBRARY DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_app
  ARCHIVE DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_app)

IF(WIN32)
  SET(TALIPOT_FIXUP_BUNDLE ON CACHE BOOL "Enable / disable the gathering and copy of Talipot DLL dependencies into the install bin folder")
  # That simple install command does the magic of gathering all required dll dependencies
  # in order to run Talipot and copy them to the bin install folder (thanks to the CMake module named BundleUtilities)
  # In the case where a dll dependency is not automatically found, you must add its path to the CMAKE_LIBRARY_PATH variable.
  IF(TALIPOT_FIXUP_BUNDLE)
    # copy required Qt plugins
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/imageformats DESTINATION ${TalipotBinInstallDir})
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/platforms DESTINATION ${TalipotBinInstallDir})
    IF(EXISTS ${QT_PLUGINS_DIR}/styles)
      INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/styles DESTINATION ${TalipotBinInstallDir})
    ENDIF(EXISTS ${QT_PLUGINS_DIR}/styles)

    # then schedule a call to FIXUP_BUNDLE at the end of the CMake install process
    # this process is configured here instead in the root CMakeLists.txt as
    # CMake inserts install commands before the ones defined in sub directories
    # and we need Talipot to be fully installed before running the bundle fixing process
    SET(APP "\${CMAKE_INSTALL_PREFIX}/bin/talipot_perspective.exe")
    SET(LIB_DIRS ${QT_BINARY_DIR} ${CMAKE_LIBRARY_PATH})
    STRING(REPLACE ";" "\;" LIB_DIRS "${LIB_DIRS}")
    GET_PROPERTY(FIXUP_BUNDLE_LIBS GLOBAL PROPERTY FIXUP_BUNDLE_LIBS)
    INSTALL(CODE
            "INCLUDE(BundleUtilities)
             FIXUP_BUNDLE(\"${APP}\" \"${FIXUP_BUNDLE_LIBS}\" \"${LIB_DIRS}\")"
    )
    # When generating Talipot installers using MSYS2, we need to bundle OpenSSL
    # libraries in order for the Geographic view to work correctly.
    # As Qt5 does not link directly to OpenSSL for license issue and thus use
    # a dynamic loading approach, we are forced to use the following trick to copy
    # the needed dlls into the Talipot install folder.
    IF(MINGW AND EXISTS ${MINGW_BIN_PATH}/openssl.exe)
      INSTALL(CODE
              "INCLUDE(BundleUtilities)
               FILE(COPY \"${MINGW_BIN_PATH}/openssl.exe\" DESTINATION \"\${CMAKE_INSTALL_PREFIX}/bin/\")
               FIXUP_BUNDLE(\"\${CMAKE_INSTALL_PREFIX}/bin/openssl.exe\" \"\" \"${LIB_DIRS}\")
               FILE(REMOVE \"\${CMAKE_INSTALL_PREFIX}/bin/openssl.exe\")"
      )
    ENDIF(MINGW AND EXISTS ${MINGW_BIN_PATH}/openssl.exe)
  ENDIF(TALIPOT_FIXUP_BUNDLE)
ENDIF(WIN32)
