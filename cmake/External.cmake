# #############################################################################
# External.cmake - External dependencies configuration
# #############################################################################
#
# This file includes all external package configurations.
# Packages are downloaded from the ares-external GitHub repository via algo-utils.
#
# Requires: ALGO_UTILS_CMAKE set in CMakeLists.txt
#
# #############################################################################

# Downloads go to external/downloads/ to keep them separate from config files
set(EXTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/downloads")

# ==============================================================================
# Boost (from algo-utils)
# ==============================================================================
# Note: Boost.System and Boost.Interprocess are header-only (Boost 1.69+)
# They're available via Boost::headers target
include(${ALGO_UTILS_CMAKE}/external/Boost.cmake)

# ==============================================================================
# NATS (from algo-utils)
# ==============================================================================
include(${ALGO_UTILS_CMAKE}/external/NATS.cmake)

message(STATUS "External dependencies configured")
