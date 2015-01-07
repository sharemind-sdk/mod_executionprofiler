#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(SHAREMIND_MOD_EXECUTIONPROFILER CXX)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeIncludes/"
                      ${CMAKE_MODULE_PATH})

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

ADD_COMPILE_OPTIONS(
  "-std=c++11" "-Wall" "-Wextra" "-O2" "-fPIC"
  "$<$<NOT:$<STREQUAL:$<CONFIGURATION>,Release>>:-ggdb>"
  "$<$<NOT:$<STREQUAL:$<CONFIGURATION>,Release>>:-fno-omit-frame-pointer>"
  "$<$<STREQUAL:$<CONFIGURATION>,Release>:-DNDEBUG>"
  "$<$<STREQUAL:$<CONFIGURATION>,Release>:-fomit-frame-pointer>"
)

FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibExecutionProfiler REQUIRED)
FIND_PACKAGE(SharemindLibmodapi REQUIRED)
FIND_PACKAGE(SharemindVmGlobals REQUIRED)

INCLUDE_DIRECTORIES(
    ${SharemindCxxHeaders_INCLUDE_DIRS}
    ${SharemindLibExecutionProfiler_INCLUDE_DIRS}
    ${SharemindLibmodapi_INCLUDE_DIRS}
    ${SharemindVmGlobals_INCLUDE_DIRS}
)

ADD_DEFINITIONS(
    ${SharemindCxxHeaders_DEFINITIONS}
    ${SharemindLibExecutionProfiler_DEFINITIONS}
    ${SharemindLibmodapi_DEFINITIONS}
    ${SharemindVmGlobals_DEFINITIONS}
)

FILE(GLOB SHAREMIND_MOD_EXECUTIONPROFILER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB SHAREMIND_MOD_EXECUTIONPROFILER_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

ADD_LIBRARY(sharemind_mod_executionprofiler MODULE ${SHAREMIND_MOD_EXECUTIONPROFILER_SOURCES}
                                            ${SHAREMIND_MOD_EXECUTIONPROFILER_HEADERS})
TARGET_LINK_LIBRARIES(sharemind_mod_executionprofiler
    ${SharemindCxxHeaders_LIBRARIES}
    ${SharemindLibExecutionProfiler_LIBRARIES}
    ${SharemindVmGlobals_LIBRARIES}
)

INSTALL(TARGETS sharemind_mod_executionprofiler LIBRARY DESTINATION "lib")
