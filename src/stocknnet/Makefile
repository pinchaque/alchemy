ROOT = ../..

SOURCES = \
	DatasetGeneratorBasic.cpp \
	DatasetGeneratorPrice.cpp \
	PopulateDataPrice.cpp \
	PopulateDataMA.cpp \
	PopulateDataPSAR.cpp \
	PopulateDataRSI.cpp \
	ProfileIO.cpp \
	ProfileMetaDataStream.cpp \

TEST_SOURCES = \
	TestDatasetGeneratorBasic.cpp \
	TestDatasetGeneratorPrice.cpp \
	TestPopulateDataPrice.cpp \
	TestPopulateDataMA.cpp \
	TestPopulateDataPSAR.cpp \
	TestPopulateDataRSI.cpp \
	TestProfileIO.cpp \
	TestProfileMetaDataStream.cpp \

include $(ROOT)/mk/buildlib.mk

LIBS += -lnnet -lstockdata -lstockalg -lautil
