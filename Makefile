main: 
	g++ --std=c++1z -Wall -g  -I ./src -L ./lib -o bin/main src/*.cpp   

run: 
	./bin/main.exe

