# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Prefix Makefile
# #############################################################################
#
# File Name       : MakePrefix.mk
#
# File Description: Makefile prefix inclusion file.
#
# Revision History: 1992-08-04 --- Creation as .MASCaPS/premake.mk.
#                       Michael L. Brock
#                   2000-01-13 --- Split into as .MASCaPS/MakePrefix.mk.
#                       Michael L. Brock
#                   2015-03-27 --- Modified as .MASCaPS/MakePrefix.mk.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 1992 - 2023.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

# #############################################################################

TMP_FILE_NAME	:=	${lastword ${MAKEFILE_LIST}}
TMP_DIR_NAME_1	:=	${shell dirname ${TMP_FILE_NAME}}
TMP_DIR_NAME_2	:=	${shell dirname ${TMP_DIR_NAME_1}}

LINK_STATIC_CPP	:=	-static-libstdc++
LINK_STATIC_GCC	:=	-static-libgcc
LINK_STATIC_STD	:=	${LINK_STATIC_CPP} ${LINK_STATIC_GCC}
LINK_STATIC_BIN	:=	-static ${LINK_STATIC_STD}
LINK_STATIC	 =

CPP_BUILD_TYPE	=

LIB_BASE	=	/usr/lib64

VER_BOOST	=	boost_1_54_0
VER_BOOST_FS	=	3
VER_TIBRV	=	8.1
#VER_XERCES	=	xercesc-3.0.1
VER_XERCES	=	xercesc
VER_PCAP	=	pcap

INC_BASIC_DIR	=	./
INC_OTHER_DIR	=	/usr/include
#INC_BOOST_DIR	=       /usr/include/boost/${VER_BOOST}/include
INC_BOOST_DIR	=       /usr/include
INC_TIBRV_DIR	=	/opt/tibco/tibrv/${VER_TIBRV}/include
INC_LBM_DIR	=	/home/brockm/Downloads/29West/UMS_5.0/Linux-glibc-2.5-x86_64/include
INC_CELOX_DIR	=	${VER_CELOX}/share/celoxica/include
INC_XERCES_DIR	=	/usr/include
INC_PCAP_DIR	=	/usr/include/${VER_PCAP}

LIB_BASIC_DIR	=	./
LIB_OTHER_DIR	=	/usr/lib64
#LIB_BOOST_DIR	=	${LIB_BASE}/${VER_BOOST}
LIB_BOOST_DIR	=	${LIB_OTHER_DIR}
LIB_TIBRV_DIR	=	/opt/tibco/tibrv/${VER_TIBRV}/lib
LIB_LBM_DIR	=	/home/brockm/Downloads/29West/UMS_5.0/Linux-glibc-2.5-x86_64/lib
LIB_XERCES_DIR	=	${LIB_BASE}
LIB_PCAP_DIR	=	${LIB_BASE}

INC_SSL_DIR	=	/usr/include/openssl
LIB_SSL_DIR	=	/usr/lib64
LIB_SSL_NAMES_A	=
LIB_SSL_NAMES_SO=	ssl

INC_CRYPTO_DIR	=
LIB_CRYPTO_DIR	=	/usr/lib64
LIB_CRYPTO_NAMES_A	=
LIB_CRYPTO_NAMES_SO	=	crypto

INC_PROTOBUF_DIR	=	/usr/include
LIB_PROTOBUF_DIR	=	/usr/lib64
LIB_PROTOBUF_NAMES_A	=
LIB_PROTOBUF_NAMES_SO	=	protobuf

INC_PROTOBUF_C_DIR	=	/usr/include/google/protobuf-c
LIB_PROTOBUF_C_DIR	=	/usr/lib64
LIB_PROTOBUF_C_NAMES_A	=
LIB_PROTOBUF_C_NAMES_SO	=	protobuf-c

INC_NATS_DIR		=	${MASCaPS_DIR_PARENT}/../nats-io/nats.c/src
LIB_NATS_DIR		=	${MASCaPS_DIR_PARENT}/../nats-io/nats.c/___MLB_BuildDir/lib
LIB_NATS_NAMES_A	=	nats_static
LIB_NATS_NAMES_SO	=	nats

# Not supported in g++ 4.1.
#			-Woverlength-strings \

#			-DBOOST_FILESYSTEM_VERSION=${VER_BOOST_FS} \

# ----- C preprocessor specific flags --- for C & C++.
CPPFLAGS	+=	\
			${CPP_STD_VER} \
			-rdynamic \
			-D__STDC_FORMAT_MACROS \
			${DEF_CELOX_CPP} \
			${CPP_BUILD_TYPE} \
			-I ./ \
			-I ${INC_BASIC_DIR} \
			-I ../include \
			${CPPFLAGS_ADDED} \
			-I${INC_CRYPTO_DIR}	\
			-I${INC_SSL_DIR}	\
			-I${INC_NATS_DIR}	\
			-I${INC_PROTOBUF_DIR}	\
			-I${INC_PROTOBUF_C_DIR}	\
			-I ${INC_OTHER_DIR} \
			-I ${INC_LBM_DIR} \
			-I ${INC_TIBRV_DIR} \
			-I ${INC_BOOST_DIR} \
			-I ${INC_CELOX_DIR} \
			-I ${INC_PCAP_DIR} \
			-I ${INC_XERCES_DIR} \
			-D_POSIX_PTHREAD_SEMANTICS \
			-Wall \
			-W \
			-Wextra \
			-Winit-self \
			-fmessage-length=0 \
			-Wno-deprecated-declarations \
			-Wno-write-strings \
			-Wredundant-decls \
			-Wpointer-arith \
			-Wno-conversion \
			-Wno-strict-aliasing \
			-pthread \
			-m64

# ----- C++ specific flags
CXXFLAGS	+=	\
			-Wno-invalid-offsetof \
			-Woverloaded-virtual

# ----- C specific flags
CFLAGS		+=	\
			-D_GNU_SOURCE

OTHER_LIBS	=

BOOST_LIBS	=	\
			${LIB_BOOST_DIR}/libboost_filesystem.a   \
			${LIB_BOOST_DIR}/libboost_date_time.a \
			${LIB_BOOST_DIR}/libboost_thread.a \
			${LIB_BOOST_DIR}/libboost_regex.a \
			${LIB_BOOST_DIR}/libboost_system.a

TIBRV_LIB	=	\
			${LIB_TIBRV_DIR}/libtibrvcmq64.a	\
			${LIB_TIBRV_DIR}/libtibrvcm64.a		\
			${LIB_TIBRV_DIR}/libtibrvft64.a		\
			${LIB_TIBRV_DIR}/libtibrvcpp64.a	\
			${LIB_TIBRV_DIR}/libtibrv64.a

LBM_LIBS	=	\
			liblbm.a

MLB_LIB_NAMES	=	\
			NatsWrapper	\
			MFStore		\
			Logger		\
			Utility

MLB_LIB_FULL	=	\
			${addsuffix .a,${addprefix ${MASCaPS_TARGET_LIB}/lib,${MLB_LIB_NAMES}}}

LDLIBS		=	\
			-Bstatic	\
			${addprefix -l,${MLB_LIB_NAMES}}	\
			${addprefix -l,${LIB_NATS_NAMES_A}}	\
			-Bdynamic	\
			${addprefix -l,${LIB_CRYPTO_NAMES_SO}}	\
			${addprefix -l,${LIB_SSL_NAMES_SO}}	\
			${addprefix -l,${LIB_PROTOBUF_NAMES_SO}}	\
			${addprefix -l,${LIB_PROTOBUF_C_NAMES_SO}}	\
			${TIBRV_LIBS}	\
			${OTHER_LIBS}	\
			${BOOST_LIBS}	\
			-lm		\
			-lrt		\
			$(OTHER_LIBS)	\
			${BOOST_LIBS}
	
LDFLAGS		+=	\
			${LINK_STATIC}		\
			-ldl			\
			-lpthread		\
			-lm			\
			-lrt			\
			-L /usr/lib64		\
			-L ${MASCaPS_TARGET_LIB}\
			${addprefix -L,${LIB_NATS_DIR}}	\
			${addprefix -L,${LIB_CRYPTO_DIR}}	\
			${addprefix -L,${LIB_SSL_DIR}}	\
			${addprefix -L,${LIB_PROTOBUF_DIR}}	\
			${addprefix -L,${LIB_PROTOBUF_C_DIR}}	\
			-L ${LIB_PCAP_DIR}	\
			-L ${LIB_BOOST_DIR}	\
			-L ${LIB_XERCES_DIR}

TARGET_LIBS	=

TARGET_BINS	=

# #############################################################################

