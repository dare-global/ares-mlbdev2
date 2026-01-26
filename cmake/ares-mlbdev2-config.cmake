# #############################################################################
# ares-mlbdev2 CMake Config File
# #############################################################################
#
# This file is installed with ares-mlbdev2 and provides the ares::mlbdev2
# IMPORTED target with all transitive dependencies properly declared.
#
# Usage:
#   find_package(ares-mlbdev2 REQUIRED CONFIG)
#   target_link_libraries(myapp PRIVATE ares::mlbdev2)
#
# #############################################################################

include(CMakeFindDependencyMacro)

# Compute the installation prefix from this file's location
# This file is at: <prefix>/lib/cmake/ares-mlbdev2/ares-mlbdev2-config.cmake
get_filename_component(_MLBDEV2_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)

# -----------------------------------------------------------------------------
# Find transitive dependencies
# -----------------------------------------------------------------------------

# Boost - components with compiled libraries
# Note: system and interprocess are header-only in Boost 1.69+
find_dependency(Boost COMPONENTS 
    thread 
    filesystem 
    chrono 
    date_time 
    regex 
    atomic
)

# NATS - NatsWrapper wraps the NATS C client API
# The cnats config already includes OpenSSL in its INTERFACE_LINK_LIBRARIES
find_dependency(cnats)

# -----------------------------------------------------------------------------
# Create the ares::mlbdev2 imported target
# -----------------------------------------------------------------------------

if(NOT TARGET ares::mlbdev2)
    add_library(ares::mlbdev2 INTERFACE IMPORTED)
    
    # Collect static libraries in correct link order
    # (most dependent first, dependencies last)
    set(_MLBDEV2_LIBS "")
    foreach(_LIB MFStore Logger NatsWrapper Utility)
        list(APPEND _MLBDEV2_LIBS "${_MLBDEV2_ROOT}/lib/lib${_LIB}.a")
    endforeach()
    
    # Set target properties
    # Link order matters for static libraries: our libs first, then dependencies
    # Note: Boost::headers includes system/interprocess (header-only since Boost 1.69+)
    set_target_properties(ares::mlbdev2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_MLBDEV2_ROOT}/include"
        INTERFACE_LINK_LIBRARIES 
            "${_MLBDEV2_LIBS};cnats::nats_static;Boost::headers;Boost::thread;Boost::filesystem;Boost::chrono;Boost::date_time;Boost::regex;Boost::atomic"
    )
endif()

# Cleanup temporary variables
unset(_MLBDEV2_ROOT)
unset(_MLBDEV2_LIBS)
