CXX = g++ -g -std=c++11
CC = gcc -g

INCS = -I. -I./common -I./vmfile

LIBS =
LIBS += -lglog -lgflags
LIBS += -lthrift -lcurl
LIBS += -lpthread -lm -lrt -lsnappy -lz -lbz2 -lbsd -lcrypto -ljson

# common src code
COMMON_DIR = ./common
COMMON_CPPFILES := $(foreach dir, $(COMMON_DIR), $(wildcard $(dir)/*.cpp))
COMMON_CFILES := $(foreach dir, $(COMMON_DIR), $(wildcard $(dir)/*.c))
COMMON_OBJECTS := $(patsubst %.cpp, %.o, $(COMMON_CPPFILES)) $(patsubst %.c, %.o, $(COMMON_CFILES))

VMFILE_DIR = ./vmfile
VMFILE_CPPFILES := $(foreach dir, $(VMFILE_DIR), $(wildcard $(dir)/*.cpp))
VMFILE_CFILES := $(foreach dir, $(VMFILE_DIR), $(wildcard $(dir)/*.c))
VMFILE_OBJECTS := $(patsubst %.cpp, %.o, $(VMFILE_CPPFILES)) $(patsubst %.c, %.o, $(VMFILE_CFILES))

exe = test

OBJS := $(VMFILE_OBJECTS) $(COMMON_OBJECTS)

all : $(exe)

test: test.o $(OBJS)
	$(CXX) $^ -o $@ $(LIBS)

%.o : %.cpp
	$(CXX) $(INCS) $(CXXFLAGS) -c $< -o $@

%.o : %.c
	$(CC) $(INCS) $(CFLAGS) -c $< -o $@

.PHONY: all histogram

clean:
	rm -f *.o $(exe) $(OBJS)