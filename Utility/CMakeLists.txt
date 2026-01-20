# #############################################################################
# Utility Library CMake Configuration
# #############################################################################

set(UTILITY_SOURCES
    CheckCmdLineArgs.cpp
    CriticalEventHandler.cpp
    CurrentPath.cpp
    DemangleCppName.cpp
    EnvElement.cpp
    GetCmdLineArg.cpp
    GetCmdLineArgRegex.cpp
    GetCmdLineHelp.cpp
    GetEnvironment.cpp
    GetSystemErrorCode.cpp
    HostName.cpp
    IoCharInfo.cpp
    Ita2ToAscii.cpp
    MapUtf8ToAscii.cpp
    MemICmp.cpp
    nstrcat.cpp
    nstrcpy.cpp
    PageSize.cpp
    ParseCfgFile.cpp
    ParseCfgItem.cpp
    ParseCsv.cpp
    ParseLine.cpp
    PathMetaChar.cpp
    PathNameCanonicalizeSlashes.cpp
    PathNameGetDirectory.cpp
    PathNameGetExtension.cpp
    PathNameGetFileName.cpp
    PathNameSlash.cpp
    ProcessId.cpp
    ReadFile.cpp
    ResolveCmdLineArg.cpp
    ResourceLimits.cpp
    RsrcUsage.cpp
    Sleep.cpp
    StringCharsToOneChar.cpp
    StringCompareIgnoreCase.cpp
    StringLowerCase.cpp
    StringOneSpace.cpp
    StringPadLeft.cpp
    StringPadRight.cpp
    StringSplit.cpp
    StringTrim.cpp
    StringTrimLeft.cpp
    StringTrimRight.cpp
    StringUpperCase.cpp
    StrICmp.cpp
    StrNICmp.cpp
    ThreadId.cpp
    ThrowErrno.cpp
    ThrowSystemError.cpp
    TimeSpec.cpp
    TimeSupport.cpp
    TimeT.cpp
    TimeTM.cpp
    TimeVal.cpp
    ToStreamErrno.cpp
    ToStreamSystemError.cpp
    ToStringErrno.cpp
    ToStringSystemError.cpp
    UniqueId.cpp
    UserName.cpp
    VersionNumber.cpp
    XLateEscapeChars.cpp
)

add_library(Utility ${UTILITY_SOURCES})

target_include_directories(Utility
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add Boost include directories if using external Boost
if(Boost_FOUND AND Boost_INCLUDE_DIRS)
    target_include_directories(Utility PUBLIC ${Boost_INCLUDE_DIRS})
endif()

target_link_libraries(Utility
    PUBLIC
        Threads::Threads
        ${CMAKE_DL_LIBS}
)

# Link Boost libraries - handle both external and find_package cases
if(Boost_FOUND)
    if(Boost_LIBRARIES)
        # External Boost (from ares-external or BOOST_ROOT)
        target_link_libraries(Utility PUBLIC ${Boost_LIBRARIES})
    elseif(TARGET Boost::filesystem)
        # find_package Boost with imported targets
        target_link_libraries(Utility PUBLIC Boost::filesystem Boost::regex Boost::system)
    endif()
endif()

set_target_properties(Utility PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME Utility
)

# Installation
install(TARGETS Utility
    EXPORT MlbDev2Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
