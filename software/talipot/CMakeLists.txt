SET(SRCS
  src/main.cpp
  src/PerspectiveItemWidget.cpp
  src/PluginErrorReport.cpp
  src/PluginsCenter.cpp
  src/PluginInformationListItem.cpp
  src/MainWindow.cpp
  src/SplashScreen.cpp
  src/WelcomePage.cpp
  src/PerspectiveProcessHandler.cpp
  src/PerspectiveCrashHandler.cpp
  src/FormPost.cpp
  src/PerspectiveSelectionDialog.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_UI(UI_HEADERS
  designer/PerspectiveItem.ui
  designer/PluginsCenter.ui
  designer/MainWindow.ui
  designer/WelcomePage.ui
  designer/PluginInformationListItem.ui
  designer/PerspectiveCrashHandler.ui
  designer/PerspectiveSelectionDialog.ui
)

IF(TALIPOT_BUILD_PYTHON_COMPONENTS)
  ADD_DEFINITIONS("-DTALIPOT_BUILD_PYTHON_COMPONENTS")
ENDIF(TALIPOT_BUILD_PYTHON_COMPONENTS)

QTX_WRAP_CPP(MOC_SRCS
  include/PerspectiveItemWidget.h
  include/PluginErrorReport.h
  include/PluginsCenter.h
  include/MainWindow.h
  include/WelcomePage.h
  include/PluginInformationListItem.h
  include/PerspectiveProcessHandler.h
  include/PerspectiveCrashHandler.h
  include/FormPost.h
  include/PerspectiveSelectionDialog.h
)

QTX_ADD_RESOURCES(RCC_SRCS resources/TalipotAgent.qrc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(include ${CMAKE_CURRENT_BINARY_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(WIN32)

STRING(TIMESTAMP CURRENT_YEAR "%Y")

INCLUDE(generate_product_version)
generate_product_version(
   VersionFiles
   NAME "Talipot Agent"
   ICON ${PROJECT_SOURCE_DIR}/library/talipot-gui/resources/icons/talipot.ico
   VERSION_MAJOR ${TalipotMajorVersion}
   VERSION_MINOR ${TalipotMinorVersion}
   VERSION_PATCH ${TalipotReleaseVersion}
   COMPANY_NAME "The Talipot developers"
   COMPANY_COPYRIGHT "(C) Copyright ${CURRENT_YEAR}"
   ORIGINAL_FILENAME "talipot.exe"
)

SET(SRCS ${SRCS} ${VersionFiles})

ENDIF(WIN32)

ADD_EXECUTABLE(talipot ${SRCS} ${UI_HEADERS} ${MOC_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(talipot crash_handling ${LibTalipotCoreName} ${LibTalipotOGLName} ${LibTalipotGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS talipot
  RUNTIME DESTINATION ${TalipotBinInstallDir} COMPONENT talipot_app
  LIBRARY DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_app
  ARCHIVE DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_app)

IF(LINUX)

  SET(TALIPOT_LINUX_DESKTOP_REGISTRATION ON CACHE BOOL "Register Talipot application and set file associations on Linux desktops ? [ON|OFF]")

  IF(TALIPOT_LINUX_DESKTOP_REGISTRATION)

    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/Talipot.desktop.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Talipot-${TalipotVersion}.desktop)

    INSTALL(CODE
      "EXECUTE_PROCESS(COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime/packages
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)"
    )

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlpx.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlp.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlpgz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlpz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlpb.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlpbgz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-talipot-tlpbz.xml
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime/packages)

    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Talipot-${TalipotVersion}.desktop
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/talipot-icon.png
            RENAME talipot.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/talipot-file-icon.png
            RENAME application-x-talipot.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/talipot-icon.png
            RENAME talipot.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/talipot-file-icon.png
            RENAME application-x-talipot.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)


    INSTALL(CODE
      "EXECUTE_PROCESS(COMMAND update-desktop-database ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications
                       COMMAND update-mime-database ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime
                       OUTPUT_QUIET ERROR_QUIET)"
    )

  ENDIF(TALIPOT_LINUX_DESKTOP_REGISTRATION)

ENDIF(LINUX)
