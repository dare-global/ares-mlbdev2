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

# Link dependencies
# Boost targets provided by algo-utils/cmake/external/Boost.cmake
# Note: Boost::system is header-only since Boost 1.69, use Boost::headers
target_link_libraries(Utility
    PUBLIC
        Threads::Threads
        ${CMAKE_DL_LIBS}
        Boost::headers
        Boost::filesystem
        Boost::regex
        Boost::thread
        Boost::chrono
        Boost::date_time
        Boost::atomic
)

set_target_properties(Utility PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME Utility
)

# Installation
install(TARGETS Utility
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
