ROOT = ../..

SOURCES = \
	FileStockDataSource.cpp \
	MetaFileStockDataSource.cpp \
	IStockDataFile.cpp \
	OStockDataFile.cpp \
	Portfolio.cpp \
	RangeData.cpp \
	RangeDataAlg.cpp \
	StockDataRetriever.cpp \
	StockDataStream.cpp \
	StockMetaDataStream.cpp \
	StockListStream.cpp \
	StockTimeUtil.cpp \
	YahooStockDataSource.cpp \

TEST_SOURCES = \
	TestBasicData.cpp \
	TestFileStockDataSource.cpp \
	TestIStockDataFile.cpp \
	TestOStockDataFile.cpp \
	TestPortfolio.cpp \
	TestRangeData.cpp \
	TestRangeDataAlg.cpp \
	TestStockID.cpp \
	TestStockTime.cpp \
	TestStockDataRetriever.cpp \
	TestStockDataStream.cpp \
	TestStockMetaDataStream.cpp \
	TestStockListStream.cpp \
	TestStockTimeUtil.cpp \
	TestYahooStockDataSource.cpp \

include $(ROOT)/mk/buildlib.mk

LIBS += -lautil -lboost_date_time-gcc -lboost_filesystem-gcc
