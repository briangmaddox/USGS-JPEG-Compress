# Generated automatically from Makefile.in by configure.
SHELL			=	/bin/sh

prefix			=	/home/cbilder
exec_prefix		=	${prefix}
host_os			=	linux-gnu
srcdir			=	.
top_srcdir		=	.
enable_debug		=	no

# Where to find includes for libraries that Benchtest depends on.
INCPATHS = -I$(prefix)/include

# libs that the program depends on
LIBS = -lMiscUtils -lImageLib -ljpeg -ltiff 

# lib dirs that the program needs
LIBDIRS = -L$(prefix)/lib

ifeq ($(enable_debug),yes)
DEBUG= -g -Wall
else
DEBUG= -O3
endif


CC= gcc
CXX= c++
CXXFLAGS= $(NOUCHAR) $(DEBUG) $(INCPATHS)
RANLIB=ranlib

SRCS=   JPEGCompression.cpp \
	JPEGDecompression.cpp \
	test.cpp

OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: compresstime
	@echo "WhooHoo!."

compresstime: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBDIRS) $(OBJS) -o compresstime $(LIBS)

clean::
	rm -f compresstime core *~ $(OBJS)

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log 