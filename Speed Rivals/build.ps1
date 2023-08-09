# Compile step
g++ -c main.cpp -I "C:\Users\hamma\Documents\C++ Libraries\SFML-2.5.1\include" -DSFML_STATIC

# Link step
g++ main.o -o main -L "C:\Users\hamma\Documents\C++ Libraries\SFML-2.5.1\lib"  -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows

# Clean step
Remove-Item main.o

.\main.exe