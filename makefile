all: tetris.exe

tetris.exe: main.o board.o piece.o
	g++ main.o board.o piece.o -o tetris.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

main.o: main.cpp
	g++ -c main.cpp -w

board.o: board.cpp
	g++ -c board.cpp -w

piece.o: piece.cpp
	g++ -c piece.cpp -w

clean:
	rm -rf *o tetris.exe
