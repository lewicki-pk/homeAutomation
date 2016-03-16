CC=g++
CFLAGS+="-I./inc"
CXXFLAGS+="-std=c++14"

LDFLAGS+=-lmosquitto
LDFLAGS+=-lrf24-bcm

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLES=homeAutomation

all : $(EXECUTABLES)

$(EXECUTABLES): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLES) $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CXXFLAGS) $< -o $@
clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
