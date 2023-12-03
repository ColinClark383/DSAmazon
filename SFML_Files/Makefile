all: compile link

compile: 
	g++ -c ..\main.cpp -I".\SFML-2.5.1\include" -DSFML_STATIC
	g++ -c ..\trie.cpp 
	g++ -c ..\hash.cpp

link:
	g++ main.o -o main trie.o hash.o -L".\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lsfml-main
	
	
clean:
	rm -f main *.o