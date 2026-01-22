# #############################################################################
# External.cmake - External dependencies configuration
# #############################################################################
#
# This file includes all external package configurations.
# Packages are downloaded from the ares-external GitHub repository.
#
# #############################################################################

# Downloads go to external/downloads/ to keep them separate from config files
set(EXTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/downloads")

# Include AresExternal from algo-utils (includes DownloadAndExtract.cmake)
include(AresExternal)

# ==============================================================================
# Boost
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/boost/Boost.cmake)

# ==============================================================================
# NATS
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/nats.c/Nats.c.cmake)

message(STATUS "External dependencies configured")
