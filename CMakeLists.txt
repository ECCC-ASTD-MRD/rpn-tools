# The minimum version can not be 3.16 because of the LINK_LANGUAGE generator expression in librmn's rmnConfig.cmake
cmake_minimum_required(VERSION 3.20)

#----- Append EC specific module path
list(APPEND CMAKE_MODULE_PATH $ENV{EC_CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_rpn/modules)

include(ec_init)           # Initialise compilers and ec specific functions
ec_git_version()           # Get version from git state
ec_parse_manifest()        # Parse MANIFEST file

project(${NAME} DESCRIPTION "${DESCRIPTION}" LANGUAGES C Fortran)
set(PROJECT_VERSION ${VERSION}${STATE} CACHE STRING "Define the version of rpn-tools that will be baked into the various artifacts of this build" FORCE)

include(ec_compiler_presets)

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_C_STANDARD 99)

ec_build_info()            # Generate build information

#include(CTest)
#add_custom_target(check COMMAND CTEST_OUTPUT_ON_FAILURE=TRUE ${CMAKE_CTEST_COMMAND})

if (NOT rmn_FOUND)
   find_package(rmn ${rmn_REQ_VERSION} REQUIRED)
endif()

add_subdirectory(src/ip1newstyle)
add_subdirectory(src/r.crc)
add_subdirectory(src/r.date)
add_subdirectory(src/r.filetype)
add_subdirectory(src/r.ip1)
add_subdirectory(src/r.ip123)
add_subdirectory(src/misc)

#----- Packaging
ec_install_prefix(${NAME} ${PROJECT_VERSION} PACKAGE_PREFIX)   # Define package prefix  
ec_build_config()                                              # Create build configuration script
ec_prepare_ssm()                                               # Prepare ssm packaging files

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_VENDOR "ECCC")
set(CPACK_PACKAGE_CONTACT "${MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.org")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/package")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CPACK_PACKAGE_FILE_NAME "${PACKAGE_PREFIX}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${NAME}_${PROJECT_VERSION}")
include(CPack)
