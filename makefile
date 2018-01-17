CXX := g++
AR := ar
RANLIB := ranlib
CXXFLAGS := -std=c++14 -pthread -O3 -g3 -Wall -c -fmessage-length=0
LKRFLAGS := -lmp3lame

OBJ := $(addprefix cpp/lib/,MP3Encoder.o Iterator32.o PCMData.o PCMFile.o WAVFile.o AIFFFile.o base.o DataChunk.o Form.o Conversions.o transcoder.o)
OBJ_ALL := $(OBJ) cpp/lib/main.cpp

LIBFILE := libpcm2mp3
LIBS := libpcm2mp3.a libpcm2mp3.so
TESTFILE := lamer

	
.PHONY: all
all: $(LIBS)

$(LIBS) : $(OBJ)
	$(AR) cr $@ $^
	$(RANLIB) $@
	
%.o: %.c
	cd cpp/lib; $(CXX) $(CXXFLAGS) -c $@ $<
	
.PHONY: clean
clean: cleanlib 
	cd cpp/lib;rm -f *.o
	 
.PHONY: cleanlib
cleanlib: $(LIBS)
	rm -f $^

	
.PHONY: test
test: $(TESTFILE)

$(TESTFILE): $(OBJ_ALL)
	$(CXX) $(LKRFLAGS) -o $(TESTFILE) $(OBJ)