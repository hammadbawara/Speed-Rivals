# Compiler
CC := g++

# Compiler flags
CFLAGS := -std=c++11 -Wall -Wextra -pedantic -I"C:\Users\hamma\Documents\C++ Libraries\SFML-2.5.1\include" -DSFML_STATIC

# SFML library flags
SFMLFLAGS := -L"C:\Users\hamma\Documents\C++ Libraries\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows

# Source files
SOURCES := main.cpp

# Output executable name
EXECUTABLE := main

all: $(EXECUTABLE)

$(EXECUTABLE): main.o
	$(CC) main.o -o $(EXECUTABLE) $(SFMLFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f $(EXECUTABLE) main.o

run: $(EXECUTABLE)
	./$(EXECUTABLE)
