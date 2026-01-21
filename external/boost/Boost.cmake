# #############################################################################
# Boost.cmake - Download Boost from ares-external
# #############################################################################

set(BOOST_PACKAGE_NAME "boost")
set(BOOST_VERSION "1.90.0")
set(BOOST_NAME_VERSION "${BOOST_PACKAGE_NAME}-${BOOST_VERSION}")

# Download and extract from ares-external
get_from_ares_external(
    ${BOOST_PACKAGE_NAME}
    ${BOOST_VERSION}
    ""
    BOOST_INSTALL_DIR
)

# Export paths for use by the build
set(BOOST_INCLUDE_DIR "${BOOST_INSTALL_DIR}" CACHE PATH "Boost include directory" FORCE)
set(BOOST_LIBRARY_DIR "${BOOST_INSTALL_DIR}/lib" CACHE PATH "Boost library directory" FORCE)

# Mark Boost as found
set(Boost_FOUND TRUE CACHE BOOL "Boost found" FORCE)
set(Boost_INCLUDE_DIRS "${BOOST_INCLUDE_DIR}" CACHE PATH "Boost include dirs" FORCE)
set(Boost_LIBRARY_DIRS "${BOOST_LIBRARY_DIR}" CACHE PATH "Boost library dirs" FORCE)

message(STATUS "${BOOST_NAME_VERSION} configured:")
message(STATUS "  Include: ${BOOST_INCLUDE_DIR}")
message(STATUS "  Library: ${BOOST_LIBRARY_DIR}")
