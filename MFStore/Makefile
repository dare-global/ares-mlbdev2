# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
#       MFStore Library Make File
# #############################################################################
#
# File Name       : MFStore/Makefile
#
# File Description: MFStore library make file.
#
# Revision History: 2021-02-14 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2021 - 2024.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libMFStore.a

TARGET_BINS	=

SRCS		=	\
			CheckValues.cpp			\
			CreateMFStore.cpp		\
			EnsureFileBackingStore.cpp	\
			FixUpFileSizePending.cpp	\
			GetWriterAdvisoryLock.cpp	\
			MFStoreControl.cpp		\
			MFStoreSection.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
#MLB_LIB_NAMES	=	Utility
MLB_LIB_NAMES	=	\
			MFStore	\
			Logger	\
			Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

