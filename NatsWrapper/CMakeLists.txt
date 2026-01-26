# #############################################################################
# NatsWrapper Library CMake Configuration
# #############################################################################

set(NATSWRAPPER_SOURCES
    NatsConnection.cpp
    NatsContext.cpp
    NatsExceptionStatus.cpp
    NatsInbox.cpp
    NatsMsg.cpp
    NatsOptions.cpp
    NatsStatus.cpp
    NatsSubscription.cpp
)

add_library(NatsWrapper ${NATSWRAPPER_SOURCES})

target_include_directories(NatsWrapper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link NATS - cnats::nats_static includes OpenSSL in its interface
target_link_libraries(NatsWrapper
    PUBLIC
        Utility
    PRIVATE
        cnats::nats_static
)

set_target_properties(NatsWrapper PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME NatsWrapper
)

# Installation
install(TARGETS NatsWrapper
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
