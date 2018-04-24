CXX := g++
AR := ar
RANLIB := ranlib
CXXFLAGS := -std=c++14 -pthread -O3 -g3 -Wall -c -fmessage-length=0
LKRFLAGS := -lmp3lame

OBJ := $(addprefix cpp/lib/,MP3Encoder.o  Iterator32.o PCMData.o PCMFile.o WAVFile.o AIFFFile.o base.o DataChunk.o Form.o Conversions.o transcoder.o)
OBJ_ALL := $(OBJ) cpp/lib/main.cpp
OBJ_MP3 := $(addprefix cpp.mp3/, MP3File.o MP3Frame.o MP3.o)
OBJ_MP3_ALL := $(OBJ_MP3) cpp.mp3/mp3.o

LIBFILE := libpcm2mp3
LIBS := libpcm2mp3.a libpcm2mp3.so
MP3LIBS := libmp3info.a libmp3info.so
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
	cd cpp.mp3;rm -f *.o
	 
.PHONY: cleanlib
cleanlib: $(LIBS) $(MP3LIBS)
	rm -f $^

.PHONY: mp3lib
mp3lib: $(MP3LIBS)

$(MP3LIBS) : $(OBJ_MP3)
	$(AR) cr $@ $^
	$(RANLIB) $@

.PHONY: mp3
mp3: $(OBJ_MP3_ALL) 
	$(CXX) -o mp3 $(OBJ_MP3_ALL)
	
.PHONY: test
test: $(TESTFILE)

$(TESTFILE): $(OBJ_ALL)
	$(CXX) $(LKRFLAGS) -o $(TESTFILE) $(OBJ)