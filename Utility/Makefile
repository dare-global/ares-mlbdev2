# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
#       Utility Library Make File
# #############################################################################
#
# File Name       : Utility/Makefile
#
# File Description: Utility library make file.
#
# Revision History: 2000-01-13 --- Creation.
#                       Michael L. Brock
#                   2015-03-29 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2000 - 2022.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libUtility.a

TARGET_BINS	=

PENDING_SRCS	=

SRCS		=	\
			CheckCmdLineArgs.cpp		\
			CriticalEventHandler.cpp	\
			CurrentPath.cpp			\
			DemangleCppName.cpp		\
			EnvElement.cpp			\
			GetCmdLineArg.cpp		\
			GetCmdLineArgRegex.cpp		\
			GetCmdLineHelp.cpp		\
			GetEnvironment.cpp		\
			GetSystemErrorCode.cpp		\
			HostName.cpp			\
			IoCharInfo.cpp			\
			Ita2ToAscii.cpp			\
			MapUtf8ToAscii.cpp		\
			MemICmp.cpp			\
			nstrcat.cpp			\
			nstrcpy.cpp			\
			PageSize.cpp			\
			ParseCfgFile.cpp		\
			ParseCfgItem.cpp		\
			ParseCsv.cpp			\
			ParseLine.cpp			\
			PathMetaChar.cpp		\
			PathNameCanonicalizeSlashes.cpp	\
			PathNameGetDirectory.cpp	\
			PathNameGetExtension.cpp	\
			PathNameGetFileName.cpp		\
			PathNameSlash.cpp		\
			ProcessId.cpp			\
			ReadFile.cpp			\
			ResolveCmdLineArg.cpp		\
			ResourceLimits.cpp		\
			RsrcUsage.cpp			\
			Sleep.cpp			\
			StringCharsToOneChar.cpp	\
			StringCompareIgnoreCase.cpp	\
			StringLowerCase.cpp		\
			StringOneSpace.cpp		\
			StringPadLeft.cpp		\
			StringPadRight.cpp		\
			StringSplit.cpp			\
			StringTrim.cpp			\
			StringTrimLeft.cpp		\
			StringTrimRight.cpp		\
			StringUpperCase.cpp		\
			StrICmp.cpp			\
			StrNICmp.cpp			\
			ThreadId.cpp			\
			ThrowErrno.cpp			\
			ThrowSystemError.cpp		\
			TimeSpec.cpp			\
			TimeSupport.cpp			\
			TimeT.cpp			\
			TimeTM.cpp			\
			TimeVal.cpp			\
			ToStreamErrno.cpp		\
			ToStreamSystemError.cpp		\
			ToStringErrno.cpp		\
			ToStringSystemError.cpp		\
			UniqueId.cpp			\
			UserName.cpp			\
			VersionNumber.cpp		\
			XLateEscapeChars.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

