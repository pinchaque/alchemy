ROOT = ../..

SOURCES = \
	CSVPlotDisplay.cpp \
	GnuPlotDisplay.cpp \
	MAPlotDataCreator.cpp \
	Plot.cpp \
	PlotData.cpp \
	PlotDataPoint.cpp \
	PlotList.cpp \
	PlotListRangeAdjuster.cpp \
	PricePlotDataCreator.cpp \
	ProfilePlotDataCreator.cpp \
	PSARPlotDataCreator.cpp \
	ROCPlotDataCreator.cpp \
	RSIPlotDataCreator.cpp \

TEST_SOURCES = \
	TestCSVPlotDisplay.cpp \
	TestGnuPlotDisplay.cpp \
	TestMAPlotDataCreator.cpp \
	TestPlotListRangeAdjuster.cpp \
	TestPricePlotDataCreator.cpp \
	TestPSARPlotDataCreator.cpp \

include $(ROOT)/mk/buildlib.mk

LIBS += \
	-lnnet \
	-lstockalg \
	-lstockdata \
	-lautil \
