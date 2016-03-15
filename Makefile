MAKEFLAGS=-j4
CC=g++
CFLAGS+="-I./inc"
CXXFLAGS+="-std=c++14"

LDFLAGS=

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLES=homeAutomation

all : $(EXECUTABLES)

$(EXECUTABLES): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLES)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
