########################################################################
# Included Makefile for Building Executables

# A package Makefile should include this file when it is to build
# an executable. In order for this to work, the Makefile
# must define the location of the root directory, source files, 
# dependencies, and any additional compiler flags. This file sets up 
# the executable name and defines all the Makefile targets.

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

TARGET = $(BINDIR)/$(PACKAGE)
include $(ROOT)/mk/package.mk
