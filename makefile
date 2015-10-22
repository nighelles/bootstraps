all: bootstraps

bootstraps: bootstraps.o boilerplate.so
	g++ bootstraps.o -o bootstraps -ldl
	
bootstraps.o: bootstraps.cpp
	g++ -c bootstraps.cpp

boilerplate.so: boilerplate.cpp
	g++ -std=c++11 -fPIC -shared boilerplate.cpp -o boilerplate.so -lncurses
