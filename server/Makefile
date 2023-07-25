#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(CXX)),)
$(error "Please set CXX in your environment. export CXX=g++")
endif

#---------------------------------------------------------------------------------
# all directories are relative to this makefile
#---------------------------------------------------------------------------------
CPPFILES	:=
CPPDIRS		:=	core ../common .
CDIRS		:=	../common/iniparser/src
INCLUDES	:=	-I..

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS		:=
CXXFLAGS	:=	-std=c++11
DEFINES		:=	-DEMULATOR=0 -DD2KSERVER -DNULL_VALUE=0
LDFLAGS		:=	-lX11 -lXtst

#---------------------------------------------------------------------------------
export OUTPUTFILE	:=	ds2key
VPATH := $(CPPDIRS) $(CDIRS)

CFILES		:=	$(foreach dir,$(CDIRS),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(filter-out vjoy.cpp, $(foreach dir,$(CPPDIRS),$(notdir $(wildcard $(dir)/*.cpp))))

OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o)

#---------------------------------------------------------------------------------
export LD	:=	$(CXX)

.PHONY: all clean build

all: $(OUTPUTFILE)

$(OUTPUTFILE) : $(OFILES)
	$(LD) $(DEFINES) $(OFILES) $(LDFLAGS) -o $@

%.o: %.c
	$(CXX) $(DEFINES) $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(DEFINES) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

build: $(OUTPUTFILE)

clean:
	rm -rf $(OFILES) $(OUTPUTFILE)