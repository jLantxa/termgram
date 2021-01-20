CXX_FLAGS := -std=c++20 -O2 -Wall -Werror
INCLUDE := include
SRC := src
BUILD := build
TARGET := termgram

TD := td
TD_STATIC_LIBS += \
	$(TD)/libtdclient.a \
	$(TD)/libtdapi.a \
	$(TD)/libtdcore.a \
	$(TD)/libtdactor.a \
	$(TD)/libtdnet.a \
	$(TD)/libtdutils.a \
	$(TD)/libtddb.a
TD_DYNAMIC_LIBS += \
	-lsqlite3 -lz -lpthread -lcrypto -lssl

SOURCES += \
	$(SRC)/main.cpp

all:
	@mkdir -p $(BUILD)
	$(CXX) $(SOURCES) $(TD_DYNAMIC_LIBS) $(TD_STATIC_LIBS) -o $(BUILD)/$(TARGET)
