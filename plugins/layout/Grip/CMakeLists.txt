INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(Grip-${TalipotVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(Grip-${TalipotVersion}  ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Grip-${TalipotVersion} ${TalipotPluginsInstallDir})
