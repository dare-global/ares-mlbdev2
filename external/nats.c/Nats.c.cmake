# #############################################################################
# Nats.c.cmake - Download NATS C client from ares-external
# #############################################################################

set(NATS_PACKAGE_NAME "nats.c")
set(NATS_VERSION "3.11.0")
set(NATS_NAME_VERSION "${NATS_PACKAGE_NAME}-${NATS_VERSION}")

# Download and extract from ares-external
# get_from_ares_external_repo returns the archive directory (parent of extracted folder)
get_from_ares_external_repo(
    ${NATS_PACKAGE_NAME}
    ${NATS_VERSION}
    ""
    NATS_ARCHIVE_DIR
)
set(NATS_INSTALL_DIR "${NATS_ARCHIVE_DIR}/${NATS_NAME_VERSION}")

# Export paths for use by the build
set(NATS_INCLUDE_DIR "${NATS_INSTALL_DIR}/include" CACHE PATH "NATS include directory" FORCE)
set(NATS_LIBRARY_DIR "${NATS_INSTALL_DIR}/lib" CACHE PATH "NATS library directory" FORCE)
set(NATS_LIBRARY "${NATS_LIBRARY_DIR}/libnats_static.a" CACHE FILEPATH "NATS static library" FORCE)

# Mark NATS as found
set(NATS_FOUND TRUE CACHE BOOL "NATS found" FORCE)

message(STATUS "${NATS_NAME_VERSION} configured:")
message(STATUS "  Include: ${NATS_INCLUDE_DIR}")
message(STATUS "  Library: ${NATS_LIBRARY}")
