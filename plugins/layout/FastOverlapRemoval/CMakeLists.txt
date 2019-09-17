INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(FastOverlapRemoval-${TalipotVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(FastOverlapRemoval-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(FastOverlapRemoval-${TalipotVersion} ${TalipotPluginsInstallDir})
