all:
	g++ main.cpp -I libdivsufsort/build/include/ -L libdivsufsort/build/lib/ -ldivsufsort64 -Wl,-rpath=libdivsufsort/build/lib -O3 -o LCSS
