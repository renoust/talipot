INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${OGDFInclude} ${TalipotOGDFInclude})

SET(PLUGINS_SRCS
OGDFFm3.cpp
OGDFUpwardPlanarization.cpp
OGDFFrutchermanReingold.cpp
OGDFFastMultipoleMultilevelEmbedder.cpp
OGDFFastMultipoleEmbedder.cpp
OGDFVisibility.cpp
OGDFKamadaKawai.cpp
OGDFCircular.cpp
OGDFBalloon.cpp
OGDFDavidsonHarel.cpp
OGDFMMMExampleNoTwistLayout.cpp
OGDFTree.cpp
OGDFMMMExampleFastLayout.cpp
OGDFGemFrick.cpp
OGDFStressMajorization.cpp
OGDFSugiyama.cpp
OGDFDominance.cpp
OGDFMMMExampleNiceLayout.cpp
OGDFPlanarizationGrid.cpp
OGDFBertaultLayout.cpp
OGDFPivotMDS.cpp
OGDFTileToRowsPacking.cpp
OGDFPlanarizationLayout.cpp
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOGDF_DLL")

DISABLE_COMPILER_WARNINGS()

FOREACH(OGDFPlugin ${PLUGINS_SRCS})
  STRING(REGEX REPLACE ".cpp" "" PLUGIN_NAME ${OGDFPlugin})
  STRING(REGEX REPLACE ".*/" "" PLUGIN_NAME ${PLUGIN_NAME})
  ADD_LIBRARY(${PLUGIN_NAME}-${TalipotVersion} SHARED ${OGDFPlugin})
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TalipotVersion} ${LibTalipotCoreName} ${LibTalipotOGDFName} ${OGDF_LIBRARY})

  TALIPOT_INSTALL_PLUGIN(${PLUGIN_NAME}-${TalipotVersion} ${TalipotPluginsInstallDir})
ENDFOREACH()
