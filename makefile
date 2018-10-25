
ifeq ($(TGT),lib)
	SRCDIR := cpp/lib
	LIBDEPS := -lmp3lame
	LIBNAME := libpcm2mp3
	EXENAME := lamer
else ifeq ($(TGT),test)
	SRCDIR := cpp/test
	LIBDEPS := 
	LIBNAME := libmp3info
	EXENAME := mp3check
else 
	$(error Unknown project target $(TGT))
endif


LIBDIR := lib
BINDIR := bin
INCDIR := include






CXX := g++
AR := ar
RANLIB := ranlib
CXXFLAGS := -std=c++14 -pthread -O3 -g3 -Wall -c -fmessage-length=0 -fPIC
LKRFLAGS := $(LIBDEPS)


LIB := $(addprefix $(LIBDIR)/,$(LIBNAME).so $(LIBNAME).a)
EXE := $(EXENAME)

makeObjs = $(patsubst %.cpp,%.o,$(filter %.cpp, $(shell find $1 -name *.cpp)))
OBJ := $(call makeObjs,$(SRCDIR))
LIB_OBJ := $(filter-out $(SRCDIR)/$(EXE).o, $(OBJ))




MKDIR := mkdir -p
RM := rm -fr
CP := cp



.PHONY: all
all: prepare $(LIB) $(EXE)

.PHONY: lib
lib : prepare $(LIB)

.PHONY: app
app : prepare $(EXE)

.PHONY: prepare
prepare:
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(INCDIR)
	$(MKDIR) $(LIBDIR)
	$(CP) $(SRCDIR)/*.hpp $(INCDIR)


clean:
	$(RM) $(SRCDIR)/*.o

.PHONY: distclean
distclean: clean
	$(RM) $(INCDIR)
	$(RM) $(LIBDIR)
	$(RM) $(BINDIR)
	
.PHONY: list
list: 
	$(info Source dir is $(SRCDIR))
	$(info File list is $(OBJ))
	$(info Library file list is $(LIB_OBJ))

$(EXE): $(OBJ)
	$(CXX) $(LKRFLAGS) $^ -o $@ 
	
$(LIB): $(LIB_OBJ)
	$(AR) cr $@ $^
	$(RANLIB) $@
	
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $@ $<