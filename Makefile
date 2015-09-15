CXX=g++
CXXFLAGS=-std=c++0x -lwiringPi -lpthread -g -lArduiPi_OLED -Ofast
LDFLAGS=
SOURCES=gui.cpp main.cpp hw.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BIN=rpb


all: $(BIN)


$(BIN): $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) -o $(BIN)


clean:
	rm -f *.o
	rm $(BIN)
