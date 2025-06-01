CXX = g++
CXXFLAGS = -O2 -Wall -Wno-unused-but-set-variable -Wno-unused-function -Wno-write-strings -Wno-unused-result
LDFLAGS = 
SOURCES = *.cpp
TARGET = floyd_warshall
INPUT_FILE = data.txt
NUM_VERTICES = 1005
NUM_THREADS = 1

build:
	rm -f $(TARGET)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

run:
	./$(TARGET) $(INPUT_FILE) $(NUM_VERTICES) $(NUM_THREADS) 