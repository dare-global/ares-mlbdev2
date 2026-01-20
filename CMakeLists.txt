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

project(MlbDev2
    VERSION 1.0.0
    DESCRIPTION "MLB Old C++ Development Redux"
    LANGUAGES CXX
)

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

# Boost - Accept external paths from parent project (e.g., ares-external)
# Priority: BOOST_INCLUDE_DIR/BOOST_LIBRARY_DIR > BOOST_ROOT > find_package
if(BOOST_INCLUDE_DIR AND BOOST_LIBRARY_DIR)
    # Use externally provided Boost paths (e.g., from ares-external)
    message(STATUS "Using externally provided Boost:")
    message(STATUS "  Include: ${BOOST_INCLUDE_DIR}")
    message(STATUS "  Library: ${BOOST_LIBRARY_DIR}")
    set(Boost_FOUND TRUE)
    set(Boost_INCLUDE_DIRS ${BOOST_INCLUDE_DIR})
    set(Boost_LIBRARY_DIRS ${BOOST_LIBRARY_DIR})
    
    # Find individual Boost libraries
    foreach(BOOST_LIB filesystem date_time thread regex system chrono atomic)
        find_library(Boost_${BOOST_LIB}_LIBRARY
            NAMES boost_${BOOST_LIB}
            PATHS ${BOOST_LIBRARY_DIR}
            NO_DEFAULT_PATH
        )
        if(Boost_${BOOST_LIB}_LIBRARY)
            list(APPEND Boost_LIBRARIES ${Boost_${BOOST_LIB}_LIBRARY})
            message(STATUS "  Found boost_${BOOST_LIB}: ${Boost_${BOOST_LIB}_LIBRARY}")
        endif()
    endforeach()
elseif(BOOST_ROOT)
    # Use BOOST_ROOT to find Boost
    message(STATUS "Using BOOST_ROOT: ${BOOST_ROOT}")
    set(BOOST_INCLUDE_DIR ${BOOST_ROOT})
    set(BOOST_LIBRARY_DIR ${BOOST_ROOT}/lib)
    set(Boost_FOUND TRUE)
    set(Boost_INCLUDE_DIRS ${BOOST_INCLUDE_DIR})
    set(Boost_LIBRARY_DIRS ${BOOST_LIBRARY_DIR})
    
    foreach(BOOST_LIB filesystem date_time thread regex system chrono atomic)
        find_library(Boost_${BOOST_LIB}_LIBRARY
            NAMES boost_${BOOST_LIB}
            PATHS ${BOOST_LIBRARY_DIR}
            NO_DEFAULT_PATH
        )
        if(Boost_${BOOST_LIB}_LIBRARY)
            list(APPEND Boost_LIBRARIES ${Boost_${BOOST_LIB}_LIBRARY})
        endif()
    endforeach()
else()
    # Fall back to system Boost via find_package
    find_package(Boost COMPONENTS filesystem date_time thread regex system chrono atomic)
endif()

# OpenSSL
find_package(OpenSSL)

# Protobuf
find_package(Protobuf)

# NATS C Client (optional - for NatsWrapper)
# Users can set NATS_ROOT to point to the nats.c installation
if(DEFINED ENV{NATS_ROOT})
    set(NATS_ROOT $ENV{NATS_ROOT})
endif()

if(NATS_ROOT)
    set(NATS_INCLUDE_DIR "${NATS_ROOT}/src")
    set(NATS_LIBRARY_DIR "${NATS_ROOT}/build/lib")
    find_library(NATS_LIBRARY
        NAMES nats_static nats
        PATHS ${NATS_LIBRARY_DIR}
        NO_DEFAULT_PATH
    )
    if(NATS_LIBRARY)
        set(NATS_FOUND TRUE)
        message(STATUS "Found NATS: ${NATS_LIBRARY}")
    endif()
else()
    # Try to find system-installed nats
    find_library(NATS_LIBRARY NAMES nats_static nats)
    find_path(NATS_INCLUDE_DIR NAMES nats/nats.h nats.h)
    if(NATS_LIBRARY AND NATS_INCLUDE_DIR)
        set(NATS_FOUND TRUE)
        message(STATUS "Found NATS: ${NATS_LIBRARY}")
    endif()
endif()

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

if(NATS_FOUND)
    add_subdirectory(NatsWrapper)
else()
    message(STATUS "NATS not found - skipping NatsWrapper library")
    message(STATUS "  Set NATS_ROOT to point to your nats.c installation")
endif()

# #############################################################################
# Installation
# #############################################################################

include(GNUInstallDirs)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# #############################################################################
# Package Configuration
# #############################################################################

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MlbDev2ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MlbDev2Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MlbDev2Config.cmake"
    @ONLY
)

install(EXPORT MlbDev2Targets
    FILE MlbDev2Targets.cmake
    NAMESPACE MlbDev2::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MlbDev2
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MlbDev2Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MlbDev2ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MlbDev2
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
message(STATUS "  NatsWrapper:      ${NATS_FOUND}")
message(STATUS "")
message(STATUS "Dependencies:")
if(BOOST_INCLUDE_DIR AND BOOST_LIBRARY_DIR)
    message(STATUS "  Boost:            ${Boost_FOUND} (external: ${BOOST_INCLUDE_DIR})")
elseif(BOOST_ROOT)
    message(STATUS "  Boost:            ${Boost_FOUND} (BOOST_ROOT: ${BOOST_ROOT})")
else()
    message(STATUS "  Boost:            ${Boost_FOUND} (system)")
endif()
message(STATUS "  OpenSSL:          ${OPENSSL_FOUND}")
message(STATUS "  Protobuf:         ${Protobuf_FOUND}")
message(STATUS "  NATS:             ${NATS_FOUND}")
message(STATUS "")
