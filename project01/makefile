build:
	rm -f work
	g++ -std=c++17 -O2 -Wall main.cpp workmatrix.o -fopenmp -o work

valgrind:
	rm -f work
	g++ -std=c++17 -O2 -Wall main.cpp workmatrix.o -fopenmp -o work
	valgrind --tool=memcheck --leak-check=full --track-origins=yes work

workmatrix:
	rm -f workmatrix.o
	g++ -std=c++17 -O2 -Wall -c workmatrix.cpp -fopenmp
