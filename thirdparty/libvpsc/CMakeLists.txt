SET(VPSC_LIB_SRCS
    block.cpp
    blocks.cpp
    cbuffer.cpp
    constraint.cpp
    rectangle.cpp
    solve_VPSC.cpp
    variable.cpp)

IF(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

INCLUDE_DIRECTORIES(${VPSCIncludeDir})

ADD_LIBRARY(${VPSCLibrary} STATIC ${VPSC_LIB_SRCS})
