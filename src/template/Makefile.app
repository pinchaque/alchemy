ROOT = ../..

SOURCES = \
	CLASSNAME.cpp \
	Main.cpp \

include $(ROOT)/mk/buildbin.mk

LIBS += -lafwk -lautil
