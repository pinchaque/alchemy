ROOT = ../..

SOURCES = \
	Framework.cpp \
	FrameworkOptions.cpp \
	FrameworkUtils.cpp \
	PathRegistry.cpp \

TEST_SOURCES = \
	TestFrameworkUtils.cpp \
	TestPathRegistry.cpp \

include $(ROOT)/mk/buildlib.mk

LIBS += \
	-lstockdata \
	-lautil \
	-lboost_program_options-gcc \
	-lboost_filesystem-gcc \
	-lboost_date_time-gcc \
