main: 
	g++ --std=c++1z -Wall -g  -I ./src -L ./lib -o bin/main src/*.cpp  -lraylib -lopengl32 -lgdi32 -lwinmm 

run: 
	./bin/main.exe

