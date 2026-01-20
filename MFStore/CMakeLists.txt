# #############################################################################
# MFStore Library CMake Configuration
# #############################################################################

set(MFSTORE_SOURCES
    CheckValues.cpp
    CreateMFStore.cpp
    EnsureFileBackingStore.cpp
    FixUpFileSizePending.cpp
    GetWriterAdvisoryLock.cpp
    MFStoreControl.cpp
    MFStoreSection.cpp
)

add_library(MFStore ${MFSTORE_SOURCES})

target_include_directories(MFStore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MFStore
    PUBLIC
        Logger
        Utility
)

set_target_properties(MFStore PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME MFStore
)

# Installation
install(TARGETS MFStore
    EXPORT MlbDev2Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
