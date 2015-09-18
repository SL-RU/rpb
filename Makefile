CXX=g++
CXXFLAGS=-std=c++0x -lwiringPi -lpthread -g -lArduiPi_OLED -Ofast
LDFLAG=-std=c++0x -lwiringPi -lpthread -g -lArduiPi_OLED -Ofast -Wall -c
SOURCES=gui.cpp hw.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BIN=rpb


all: $(SOURCES) $(BIN)


$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN)

.cpp.o:
	$(CXX) $(LDFLAG) $< -o $@

clean:
	rm -f *.o
	rm $(BIN)
