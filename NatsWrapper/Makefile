# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
#       NatsWrapper Library Make File
# #############################################################################
#
# File Name       : NatsWrapper/Makefile
#
# File Description: NatsWrapper library make file.
#
# Revision History: 2024-08-XX --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2024.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libNatsWrapper.a

TARGET_BINS	=

SRCS		=	\
			NatsConnection.cpp	\
			NatsContext.cpp		\
			NatsExceptionStatus.cpp	\
			NatsInbox.cpp		\
			NatsMsg.cpp		\
			NatsOptions.cpp		\
			NatsStatus.cpp		\
			NatsSubscription.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
#MLB_LIB_NAMES	=	Utility
MLB_LIB_NAMES	=	NatsWrapper	\
			Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

