# #############################################################################
# MlbDev2 - MLB Old C++ Development Redux
# CMake Build Configuration
# #############################################################################
#
# Copyright Michael L. Brock 2000 - 2024.
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

cmake_minimum_required(VERSION 4.2.1)

# #############################################################################
# Default to GCC 14 if available (must be set before project())
# #############################################################################

if(NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED ENV{CC})
    find_program(GCC14_C_COMPILER gcc-14)
    if(GCC14_C_COMPILER)
        set(CMAKE_C_COMPILER "${GCC14_C_COMPILER}" CACHE FILEPATH "C compiler")
    endif()
endif()

if(NOT DEFINED CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
    find_program(GCC14_CXX_COMPILER g++-14)
    if(GCC14_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "${GCC14_CXX_COMPILER}" CACHE FILEPATH "C++ compiler")
    endif()
endif()

# #############################################################################

# Version format: YYYY.mm.dd
string(TIMESTAMP MLBDEV2_VERSION "%Y.%m.%d")

project(MlbDev2
    VERSION ${MLBDEV2_VERSION}
    DESCRIPTION "MLB Old C++ Development Redux"
    LANGUAGES CXX
)

# #############################################################################
# Build Options and External Dependencies
# #############################################################################

# Build configuration (compiler detection, OS detection, etc.)
include(cmake/BuildOptions.cmake)

# External dependencies from ares-external (Boost, NATS)
include(external/External.cmake)

# #############################################################################
# C++ Standard Configuration
# #############################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# #############################################################################
# Build Options
# #############################################################################

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(MLBDEV2_BUILD_TESTS "Build unit tests" OFF)

# #############################################################################
# Compiler Flags
# #############################################################################

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -W
        -Wextra
        -Werror
        -Winit-self
        -fmessage-length=0
        -Wno-write-strings
        -Wpointer-arith
        -Wno-conversion
        -Wno-strict-aliasing
        -Wno-invalid-offsetof
        -Woverloaded-virtual
        -pthread
        -m64
        -rdynamic
    )
    add_compile_definitions(
        __STDC_FORMAT_MACROS
        _POSIX_PTHREAD_SEMANTICS
    )
endif()

# #############################################################################
# Output Directories
# #############################################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# #############################################################################
# Find Dependencies
# #############################################################################

find_package(Threads REQUIRED)

# Boost - configured via external/boost/Boost.cmake (from ares-external)
# Find individual Boost libraries in the downloaded package
foreach(BOOST_LIB filesystem date_time thread regex system chrono atomic interprocess)
    find_library(Boost_${BOOST_LIB}_LIBRARY
        NAMES boost_${BOOST_LIB}
        PATHS ${BOOST_LIBRARY_DIR}
        NO_DEFAULT_PATH
    )
    if(Boost_${BOOST_LIB}_LIBRARY)
        list(APPEND Boost_LIBRARIES ${Boost_${BOOST_LIB}_LIBRARY})
    endif()
endforeach()

# OpenSSL - uses system OpenSSL (required for NatsWrapper TLS support)
# Install via: apt install libssl-dev (Ubuntu/Debian)
find_package(OpenSSL)

# NATS - configured via external/nats.c/Nats.c.cmake (from ares-external)

# #############################################################################
# Include Directories
# #############################################################################

# Common include directory for all targets
include_directories(${CMAKE_SOURCE_DIR}/include)

# #############################################################################
# Subdirectories (in dependency order)
# #############################################################################

add_subdirectory(Utility)
add_subdirectory(Logger)
add_subdirectory(MFStore)
add_subdirectory(NatsWrapper)

# #############################################################################
# Installation
# #############################################################################

include(GNUInstallDirs)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)


# #############################################################################
# Summary
# #############################################################################

message(STATUS "")
message(STATUS "MlbDev2 Configuration Summary:")
message(STATUS "  Version:          ${PROJECT_VERSION}")
message(STATUS "  C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix:   ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Libraries:")
message(STATUS "  Utility:          Yes")
message(STATUS "  Logger:           Yes")
message(STATUS "  MFStore:          Yes")
message(STATUS "  NatsWrapper:      Yes")
message(STATUS "")
message(STATUS "Dependencies (from ares-external):")
message(STATUS "  Boost:            ${BOOST_VERSION} (${BOOST_LIBRARY_DIR})")
message(STATUS "  NATS:             ${NATS_VERSION} (${NATS_LIBRARY_DIR})")
message(STATUS "")
message(STATUS "System Dependencies:")
message(STATUS "  OpenSSL:          ${OPENSSL_FOUND} (system)")
message(STATUS "  Threads:          Yes (pthread)")
message(STATUS "")

# #############################################################################
# Packaging - Create distributable tarball
# #############################################################################
#
# Archive naming: ares-mlbdev2-<version>-boost<boost_version>-<compiler><ver>-<build>-<os>-<os_ver>.tar.gz
# Example: ares-mlbdev2-1.0.0-boost1.83.0-gcc14-release-ubuntu-24.04.tar.gz
#
# Usage: cmake --build <build_dir> --target package
#        or: make package (after install)

# Package name includes boost version for dependency tracking
string(REPLACE "." "" BOOST_VERSION_NODOTS "${BOOST_VERSION}")
set(MLBDEV2_ARCHIVE_NAME "ares-mlbdev2-${PROJECT_VERSION}-boost${BOOST_VERSION}-${MLBDEV2_COMPILER}${MLBDEV2_COMPILER_VERSION}-release-${MLBDEV2_OS_NAME}-${MLBDEV2_OS_VERSION}")

# CPack configuration
set(CPACK_PACKAGE_NAME "ares-mlbdev2")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${MLBDEV2_ARCHIVE_NAME}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)

# Output archive to archive/, staging files to install prefix
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/archive")

include(CPack)

message(STATUS "Package archive: ${CPACK_PACKAGE_DIRECTORY}/${MLBDEV2_ARCHIVE_NAME}.tar.gz")
