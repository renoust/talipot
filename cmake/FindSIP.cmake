# Find SIP
# ~~~~~~~~
#
# SIP website: http://www.riverbankcomputing.co.uk/sip/index.php
#
# Find the installed version of SIP. FindSIP should be called after Python
# has been found.
#
# This file defines the following variables:
#
# SIP_VERSION - The version of SIP found expressed as a 6 digit hex number
#     suitable for comparison as a string.
#
# SIP_VERSION_STR - The version of SIP found as a human readable string.
#
# SIP_EXECUTABLE - Path and filename of the SIP command line executable.
#
# SIP_INCLUDE_DIR - Directory holding the SIP C++ header file.
#
# SIP_DEFAULT_SIP_DIR - Default directory where .sip files should be installed
#     into.

# Copyright (c) 2007, Simon Edwards <simon@simonzone.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_FILE(_find_sip_py FindSIP.py PATHS ${CMAKE_MODULE_PATH})

MARK_AS_ADVANCED(FORCE _find_sip_py)

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} ${_find_sip_py} OUTPUT_VARIABLE sip_config ERROR_QUIET)
IF(sip_config)
  STRING(REGEX REPLACE "^sip_version:([^\n]+).*$" "\\1" SIP_VERSION ${sip_config})
  STRING(REGEX REPLACE ".*\nsip_version_str:([^\n]+).*$" "\\1" SIP_VERSION_STR ${sip_config})
  STRING(REGEX REPLACE ".*\nsip_bin:([^\n]+).*$" "\\1" SIP_EXECUTABLE ${sip_config})
  IF(NOT SIP_DEFAULT_SIP_DIR)
    STRING(REGEX REPLACE ".*\ndefault_sip_dir:([^\n]+).*$" "\\1" SIP_DEFAULT_SIP_DIR ${sip_config})
  ENDIF(NOT SIP_DEFAULT_SIP_DIR)
  STRING(REGEX REPLACE ".*\nsip_inc_dir:([^\n]+).*$" "\\1" SIP_INCLUDE_DIR ${sip_config})
  IF(EXISTS "${SIP_EXECUTABLE}" AND EXISTS "${SIP_INCLUDE_DIR}/sip.h")
    SET(SIP_FOUND TRUE)
  ELSE()
    SET(SIP_FOUND FALSE)
    IF(NOT "${SIP_VERSION}" STREQUAL "${LAST_FOUND_SIP_VERSION}")
      MESSAGE(STATUS "The SIP Python module is installed but its development tools and headers are not. Python bindings can not be compiled with the SIP version installed on the system.")
    ENDIF(NOT "${SIP_VERSION}" STREQUAL "${LAST_FOUND_SIP_VERSION}")
  ENDIF()
ENDIF(sip_config)

IF(SIP_FOUND)
  IF(NOT "${SIP_VERSION}" STREQUAL "${LAST_FOUND_SIP_VERSION}")
    MESSAGE(STATUS "Found SIP version: ${SIP_VERSION_STR}")
  ENDIF(NOT "${SIP_VERSION}" STREQUAL "${LAST_FOUND_SIP_VERSION}")
ELSE(SIP_FOUND)
  IF(SIP_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find SIP or its development tools and headers.")
  ENDIF(SIP_FIND_REQUIRED)
ENDIF(SIP_FOUND)

SET(LAST_FOUND_SIP_VERSION "${SIP_VERSION}" CACHE INTERNAL "")
