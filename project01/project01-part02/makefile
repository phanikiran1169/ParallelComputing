build:
	rm -f work
	g++ -std=c++17 -O2 -Wall main.cpp workgraph.o -fopenmp -lpthread -o work

valgrind:
	rm -f work
	g++ -std=c++17 -O2 -Wall main.cpp workgraph.o -fopenmp -lpthread -o work
	valgrind --tool=memcheck --leak-check=full --track-origins=yes work

workgraph:
	rm -f workgraph.o
	g++ -std=c++17 -O2 -Wall -c workgraph.cpp -fopenmp -lpthread
