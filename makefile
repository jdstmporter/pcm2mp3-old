LIBDIR := lib
INCDIR := include

CXX := g++
AR := ar
RANLIB := ranlib
CXXFLAGS := -std=c++14 -pthread -O3 -g3 -Wall -c -fmessage-length=0 -fPIC
LKRFLAGS := -lmp3lame

APP_OBJ := $(addprefix cpp/lib/,MP3Encoder.o MP3Data.o MP3File.o Iterator32.o PCMData.o PCMFile.o WAVFile.o AIFFFile.o base.o DataChunk.o Form.o Conversions.o transcoder.o)
APP_OBJ_ALL := $(APP_OBJ) cpp/lib/main.cpp
APP_OUT := lamer

TEST_OBJ := $(addprefix cpp/test/,MP3File.o MP3Frame.o MP3.o Test.o)
TEST_OBJ_ALL := $(TEST_OBJ) cpp/test/mp3.o
TEST_OUT := mp3
TEST_LIBS := $(addprefix $(LIBDIR)/,libmp3info.a libmp3info.so)

MKDIR := mkdir -p
RM := rm -fr
CP := cp







.PHONY: all
all: app test

.PHONY: app
app: $(APP_OUT)

.PHONY: test
test : $(TEST_OUT)

.PHONY: testlibs
testlibs: prepare $(TEST_LIBS)


.PHONY: prepare
prepare:
	$(MKDIR) $(INCDIR)
	$(MKDIR) $(LIBDIR)
	$(CP) cpp/test/*.hpp $(INCDIR)


$(TEST_LIBS) : $(TEST_OBJ)
	$(AR) cr $@ $^
	$(RANLIB) $@

.PHONY: clib
clib : $(TEST_OBJ) cpp/test/check.o
	$(CXX) -shared -o libcmp3.so $(TEST_OBJ) cpp/test/check.o


clean:
	cd cpp/lib;$(RM) *.o
	cd cpp/test;$(RM) *.o
	cd cpp.mp3;$(RM) *.o


$(APP_OUT): $(APP_OBJ_ALL)
	$(CXX) $(LKRFLAGS) -o $(APP_OUT) $(APP_OBJ_ALL)
	

$(TEST_OUT): $(TEST_OBJ_ALL)
	$(CXX) $(LKRFLAGS) -o $(TEST_OUT) $(TEST_OBJ_ALL)
	
%.o: %.c
	cd cpp/lib; $(CXX) $(CXXFLAGS) -c $@ $<
	cd cpp/test; $(CXX) $(CXXFLAGS) -c $@ $<
