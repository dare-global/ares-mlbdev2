# #############################################################################
# Logger Library CMake Configuration
# #############################################################################

set(LOGGER_SOURCES
    LogEmitControl.cpp
    LogHandler.cpp
    LogHandlerConsole.cpp
    LogHandlerFile.cpp
    LogHandlerFileBase.cpp
    LogLevel.cpp
    LogManager.cpp
    LogTestSupport.cpp
)

# LogHandlerFileMMap.cpp is pending/optional
# set(LOGGER_PENDING_SOURCES
#     LogHandlerFileMMap.cpp
# )

add_library(Logger ${LOGGER_SOURCES})

target_include_directories(Logger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Logger
    PUBLIC
        Utility
)

set_target_properties(Logger PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME Logger
)

# Installation
install(TARGETS Logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
