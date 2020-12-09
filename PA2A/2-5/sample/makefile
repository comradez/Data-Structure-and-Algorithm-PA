all : main
main : temperature.h temperature_lib.cpp temperature.cpp
	g++ temperature.cpp temperature_lib.cpp -std=c++14 -O2 -Wall -o main
debug : temperature.h temperature_lib.cpp temperature.cpp
	g++ temperature.cpp temperature_lib.cpp -std=c++14 -Wall -g -o debug
sample : temperature.h temperature_lib.cpp temperature_sample.cpp
	g++ temperature_sample.cpp temperature_lib.cpp -std=c++14 -O2 -Wall -o sample