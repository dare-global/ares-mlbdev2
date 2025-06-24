# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
#       Logger Library Make File
# #############################################################################
#
# File Name       : Logger/Makefile
#
# File Description: Logger library make file.
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

TARGET_LIBS	=	libLogger.a

TARGET_BINS	=

PENDING_SRCS	=	\
			LogHandlerFileMMap.cpp

SRCS		=	\
			LogEmitControl.cpp		\
			LogHandler.cpp			\
			LogHandlerConsole.cpp		\
			LogHandlerFile.cpp		\
			LogHandlerFileBase.cpp		\
			LogLevel.cpp			\
			LogManager.cpp			\
			LogTestSupport.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	\
			Logger	\
			Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

