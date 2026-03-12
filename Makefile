CC=g++
CFLAG=-Wall -Wextra -ggdb -Weffc++ -Wconversion -Wsign-conversion -pedantic -std=c++20 -pedantic-errors

renderer:
	$(CC) $(CFLAG) -o build/ascii src/main.cpp src/Renderer.cpp src/Math.cpp

clean:
	rm -f build/*
