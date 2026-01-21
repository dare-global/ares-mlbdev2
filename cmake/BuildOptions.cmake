# #############################################################################
# BuildOptions.cmake - Build configuration for ares-mlbdev2
# #############################################################################

# ==============================================================================
# Compiler Detection
# ==============================================================================

# Detect compiler type
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(MLBDEV2_COMPILER "gcc")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(MLBDEV2_COMPILER "clang")
else()
    set(MLBDEV2_COMPILER "unknown")
endif()

# Get compiler major version
string(REGEX MATCH "^[0-9]+" MLBDEV2_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")

# ==============================================================================
# OS Detection
# ==============================================================================

# Default to ubuntu-24.04 for ares-external package naming
set(MLBDEV2_OS_NAME "ubuntu" CACHE STRING "OS name for package archives")
set(MLBDEV2_OS_VERSION "24.04" CACHE STRING "OS version for package archives")

# Try to detect actual OS
if(EXISTS "/etc/os-release")
    file(STRINGS "/etc/os-release" OS_RELEASE_CONTENT)
    foreach(LINE ${OS_RELEASE_CONTENT})
        if(LINE MATCHES "^ID=(.*)$")
            string(REGEX REPLACE "\"" "" DETECTED_OS_NAME "${CMAKE_MATCH_1}")
            set(MLBDEV2_OS_NAME "${DETECTED_OS_NAME}" CACHE STRING "OS name" FORCE)
        elseif(LINE MATCHES "^VERSION_ID=(.*)$")
            string(REGEX REPLACE "\"" "" DETECTED_OS_VERSION "${CMAKE_MATCH_1}")
            set(MLBDEV2_OS_VERSION "${DETECTED_OS_VERSION}" CACHE STRING "OS version" FORCE)
        endif()
    endforeach()
endif()

# ==============================================================================
# ares-external Repository
# ==============================================================================

set(ARES_EXTERNAL_REPO_URL "https://github.com/dare-global/ares-external" 
    CACHE STRING "ares-external GitHub repository URL")

# ==============================================================================
# External Package Directories
# ==============================================================================

set(EXTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external" CACHE PATH "External packages source directory")
set(EXTERNAL_DOWNLOADS_DIR "${EXTERNAL_SOURCE_DIR}/downloads" CACHE PATH "External packages download directory")

# ==============================================================================
# Build Type
# ==============================================================================

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" MLBDEV2_BUILD_TYPE)

# ==============================================================================
# Display Configuration
# ==============================================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "MlbDev2 Build Configuration")
message(STATUS "========================================")
message(STATUS "Compiler:         ${MLBDEV2_COMPILER}-${MLBDEV2_COMPILER_VERSION}")
message(STATUS "Build Type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "OS:               ${MLBDEV2_OS_NAME}-${MLBDEV2_OS_VERSION}")
message(STATUS "External Repo:    ${ARES_EXTERNAL_REPO_URL}")
message(STATUS "========================================")
message(STATUS "")
