# #############################################################################
# External.cmake - External dependencies configuration
# #############################################################################
#
# This file includes all external package configurations.
# Packages are downloaded from the ares-external GitHub repository.
#
# #############################################################################

set(EXTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external")

# Include download utilities
include(${CMAKE_SOURCE_DIR}/cmake/DownloadAndExtract.cmake)

# ==============================================================================
# Boost
# ==============================================================================
include(${EXTERNAL_SOURCE_DIR}/boost/Boost.cmake)

# ==============================================================================
# NATS
# ==============================================================================
include(${EXTERNAL_SOURCE_DIR}/nats.c/Nats.c.cmake)

message(STATUS "External dependencies configured")
