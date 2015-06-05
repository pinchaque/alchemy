########################################################################
# Included Makefile for Building Libraries

# A package Makefile should include this file when it is to build
# a shared object library. In order for this to work, the Makefile
# must define the location of the root directory, source files, 
# dependencies, and any additional compiler flags. This file sets up 
# the library name and defines all the Makefile targets.

# The variables that need to be defined are, with examples:
# ROOT = ../..
# SOURCES = Foo.cpp Bar.cpp
#
# The optionally extended variables are, with examples:
# INCLUDES += -I/opt/extra
# LIBS += -lxerces
# CPPFLAGS += -DUNIXROX
# CFLAGS += -Wshadow
# CXXFLAGS += -v
# TEST_SOURCES += TestFoo.cpp TestBar.cpp

TARGET = $(LIBDIR)/lib$(PACKAGE).so
include $(ROOT)/mk/package.mk
