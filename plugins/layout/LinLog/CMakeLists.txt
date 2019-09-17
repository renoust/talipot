INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(LinLog-${TalipotVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(LinLog-${TalipotVersion}  ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(LinLog-${TalipotVersion} ${TalipotPluginsInstallDir})
