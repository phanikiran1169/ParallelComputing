CXX = g++
MPI_CXX = mpic++
CXXFLAGS = -O2 -Wall -Wno-unused-but-set-variable -Wno-unused-function -Wno-write-strings -Wno-unused-result
LDFLAGS = -fopenmp
SOURCES = *.cpp
TARGET = floyd_warshall
INPUT_FILE = data.txt
OUTPUT_FILE = output.txt
NUM_VERTICES = 6301
NUM_THREADS = 1
NUM_PROCESSES = 1

build:
	rm -f $(TARGET)
	$(MPI_CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

run:
	mpiexec -n $(NUM_PROCESSES) ./$(TARGET) $(INPUT_FILE) $(OUTPUT_FILE) $(NUM_VERTICES) $(NUM_THREADS)