build&run: build run

build:
	g++ -g -std=c++17 -Iinclude src/main.cpp src/glad.c -lglfw -lGL -ldl -pthread -o gravity-SIM

run:
	./physics-SIM.exe
