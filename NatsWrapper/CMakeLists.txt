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
        ${NATS_INCLUDE_DIR}
)

target_link_libraries(NatsWrapper
    PUBLIC
        Utility
    PRIVATE
        ${NATS_LIBRARY}
)

if(OPENSSL_FOUND)
    target_link_libraries(NatsWrapper PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

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
