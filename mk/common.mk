########################################################################
# Assumes ROOT is set to the source tree root

ifndef ROOT
$(error Variable ROOT is not defined)
endif

PACKAGE = $(shell /bin/sh -c "pwd | sed 's/.*\///'")

ifeq ($(PACKAGE),)
$(error PACKAGE could not be determined from $(shell /bin/sh -c "pwd"))
endif


########################################################################
# Directory Definitions

SRCDIR = $(ROOT)/src
MKDIR = $(ROOT)/mk
DOCDIR = $(ROOT)/doc
BUILDDIR = $(ROOT)/build
BINDIR = $(BUILDDIR)/bin
LIBDIR = $(BUILDDIR)/lib
PKGTMPDIR = $(BUILDDIR)/tmp/$(PACKAGE)
OBJDIR = $(PKGTMPDIR)/obj
DEPDIR = $(PKGTMPDIR)/dep

# Boost directories
BOOSTDIR = /opt/boost_1_32_0
BOOSTINCDIR = $(BOOSTDIR)/include/boost-1_32
BOOSTLIBDIR = $(BOOSTDIR)/lib

# Used to create directories when build system is run
DIRS = $(BINDIR) $(LIBDIR) $(OBJDIR) $(DEPDIR)


########################################################################
# Executables

CXX = g++
CC = gcc
MOC = moc
LD := $(CXX)
AR := ar -rc
RM := rm -f
LN := ln -s
CP := cp
MV := mv
PERL := perl


########################################################################
# Build Modes

MODE_DEBUG = debug
MODE_OPT = opt
MODE_PROF = prof

# Check for supported modes
SUPPORTED_MODES := $(MODE_OPT) $(MODE_DEBUG) $(MODE_PROF)

ifndef MODE
MODE := $(MODE_OPT)
endif

ifeq (,$(findstring $(MODE),$(SUPPORTED_MODES)))
$(error Supported build MODEs: $(SUPPORTED_MODES))
endif

########################################################################
# Compiler Flags

INCLUDES = -I$(SRCDIR) -I$(BOOSTINCDIR) -I/usr/local/src -I/usr/src
LIBS =
CPPFLAGS = $(INCLUDES)
CFLAGS = -g -Wall
CXXFLAGS = -g -Wall
LDFLAGS = -L$(LIBDIR) -L$(BOOSTLIBDIR) $(LIBS) -Wl,-rpath,$(BOOSTLIBDIR)
LDSOFLAGS = -shared -L$(LIBDIR) -L$(BOOSTLIBDIR) $(LIBS) -Wl,-rpath,$(BOOSTLIBDIR)

ifeq ($(MODE),$(MODE_OPT))
CFLAGS += -O2
CXXFLAGS += -O2
#CPPFLAGS += -DNDEBUG
endif

ifeq ($(MODE),$(MODE_PROF))
CFLAGS += -pg -O2
CXXFLAGS += -pg -O2
#CPPFLAGS += -DNDEBUG
endif

# Version information for builds
VERSIONSTRING ?= build_$(shell date "+%Y%m%d")
CPPFLAGS += -DVERSIONSTRING=\"$(VERSIONSTRING)\"
