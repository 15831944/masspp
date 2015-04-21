# set
PREFIX      = /usr/local
prefix      = /usr/local
EXEC_PREFIX = ${prefix}
exec_prefix = ${prefix}
BIN_DIR     = ${exec_prefix}/bin
SHARE_DIR   = ${prefix}/share
INCLUDE_DIR = ${prefix}/include
LIB_DIR     = ${exec_prefix}/lib
TMP_LIB_DIR = ../../../lib/gtk
TMP_PLG_DIR = ../../../build/gtk/plugins
TMP_INC_DIR = ../../../include/kome
MSPP_DIR    = $(SHARE_DIR)/masspp

CXX      = g++
DEFS     = -DPACKAGE_NAME=\"Mass++\" -DPACKAGE_TARNAME=\"mass--\" -DPACKAGE_VERSION=\"1.7.5\" -DPACKAGE_STRING=\"Mass++\ 1.7.5\" -DPACKAGE_BUGREPORT=\"s11-tanaka@hhc.eisai.co.jp\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FLOAT_H=1 -DHAVE_SYS_SOCKET_H=1 -DHAVE__BOOL=1 -DHAVE_STDBOOL_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_STDLIB_H=1 -DHAVE_MALLOC=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SOCKET_H=1 -DSELECT_TYPE_ARG1=int -DSELECT_TYPE_ARG234=\(fd_set\ \*\) -DSELECT_TYPE_ARG5=\(struct\ timeval\ \*\) -DHAVE_GETTIMEOFDAY=1 -DHAVE_MKDIR=1 -DHAVE_RMDIR=1 -DHAVE_STRSTR=1 -DHAVE_STRTOL=1 -DHAVE_SELECT=1 -DHAVE_GETHOSTBYNAME=1 -DHAVE_SOCKET=1 -DMSPP_INSTALL_DIR=$(MSPP_DIR)
FLAGS    = -g -O2
LIBS     =  -L${TMP_LIB_DIR} -lMsppCoreFunctions -lMsppCoreToolkits -lMsppNumericalTools -lMsppImageManager
INCLUDES = -I../../../include

.SUFFIXES:
.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(DEFS) $(FLAGS) $(INCLUDES) -fPIC -c -o $@ $<

cppfiles := $(wildcard *.cpp)
objfiles = $(cppfiles:.cpp=.o)


.PHONY: all
all: $(objfiles)
	$(CXX) $(DEFS) $(FLAGS) $(INCLUDES) $(LIBS) -shared -o $(TMP_LIB_DIR)/libMsppGLTools.so.1.0 $(objfiles)
	ln -sf libMsppGLTools.so.1.0 $(TMP_LIB_DIR)/libMsppGLTools.so
	mkdir -p $(TMP_INC_DIR)/GLTools
	cp *.h $(TMP_INC_DIR)/GLTools
	rm -f $(TMP_INC_DIR)/GLTools/stdafx.h
	mv -f $(TMP_INC_DIR)/GLTools/GLTools.h  $(TMP_INC_DIR)

.PHONY: clean
clean:
	rm -f $(objfiles)
	rm -Rf $(TMP_INC_DIR)/GLTools
	rm -f $(TMP_INC_DIR)/GLTools.h
