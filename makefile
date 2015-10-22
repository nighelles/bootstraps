all: bootstraps

bootstraps: boilerplate.so
	g++ bootstraps.o -o bootstraps
	
bootstraps.o: bootstraps.cpp
	g++ -c bootstraps.cpp

boilerplate.so: boilerplate.cpp
	g++ -std=c++11 -fPIC -shared boilerplate.cpp -o boilerplate.so -lncurses
