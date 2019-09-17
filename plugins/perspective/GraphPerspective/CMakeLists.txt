INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(LIB_SRCS
src/AlgorithmRunner.cpp
src/AlgorithmRunnerItem.cpp
src/ExpandableGroupBox.cpp
src/ExportWizard.cpp
src/GraphPerspectiveLogger.cpp
src/GraphHierarchiesEditor.cpp
src/GraphPerspective.cpp
src/HeaderFrame.cpp
src/ImportWizard.cpp
src/OutputPanelButton.cpp
src/PanelSelectionWizard.cpp
src/PreferencesDialog.cpp
src/SearchWidget.cpp
)

QTX_ADD_RESOURCES(RCC_SRCS
resources/GraphPerspective.qrc
OPTIONS -name GraphPerspective)

QTX_WRAP_UI(UI_SRCS
designer/GraphPerspectiveLogger.ui
designer/AlgorithmRunner.ui
designer/AlgorithmRunnerItem.ui
designer/GraphHierarchiesEditor.ui
designer/GraphPerspectiveMainWindow.ui
designer/HeaderFrame.ui
designer/ImportWizard.ui
designer/ExportWizard.ui
designer/PanelSelectionWizard.ui
designer/PreferencesDialog.ui
designer/SearchWidget.ui
)

IF(TALIPOT_BUILD_PYTHON_COMPONENTS)
  ADD_DEFINITIONS("-DTALIPOT_BUILD_PYTHON_COMPONENTS")
ENDIF(TALIPOT_BUILD_PYTHON_COMPONENTS)

QTX_WRAP_CPP(MOC_SRCS
include/AlgorithmRunner.h
include/AlgorithmRunnerItem.h
include/ExpandableGroupBox.h
include/ExportWizard.h
include/GraphHierarchiesEditor.h
include/GraphPerspective.h
include/GraphPerspectiveLogger.h
include/HeaderFrame.h
include/ImportWizard.h
include/OutputPanelButton.h
include/PanelSelectionWizard.h
include/PreferencesDialog.h
include/SearchWidget.h
)

IF(TALIPOT_BUILD_PYTHON_COMPONENTS)

INCLUDE_DIRECTORIES(${TalipotPythonInclude})

SET(LIB_SRCS
${LIB_SRCS}
src/PythonPanel.cpp
)

QTX_WRAP_UI(UI_SRCS
designer/PythonPanel.ui
)

QTX_WRAP_CPP(MOC_SRCS
include/PythonPanel.h
)

ENDIF(TALIPOT_BUILD_PYTHON_COMPONENTS)

ADD_LIBRARY(GraphPerspective-${TalipotVersion} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TalipotVersion} ${LibTalipotCoreName} ${LibTalipotOGLName} ${LibTalipotGUIName})
TARGET_LINK_LIBRARIES(GraphPerspective-${TalipotVersion} ${QT_LIBRARIES})
IF(TALIPOT_BUILD_PYTHON_COMPONENTS)
TARGET_LINK_LIBRARIES(GraphPerspective-${TalipotVersion} ${LibTalipotPythonName})
ENDIF(TALIPOT_BUILD_PYTHON_COMPONENTS)

TALIPOT_INSTALL_PLUGIN(GraphPerspective-${TalipotVersion} ${TalipotPerspectivePluginsInstallDir})

