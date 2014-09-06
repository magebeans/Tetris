#ifndef BOARD_H
#define BOARD_H
#include "piece.h"

enum {PLACE_OK=0, PLACE_OUT_BOUNDS, PLACE_ROW_FILLED, PLACE_OVERLAP, PLACE_FULL, PLACE_BAD};

class Board{
private:
	int grid[24][10];
	int xgrid[24][10];
	int widths[24];
	int xwidths[24];
	int heights[10];
	int xheights[10];
	int score;
	Point offset;
public:
	Board();
	int place(Piece p, Point loc);
	int drop(Piece p, Point pos);
	void display();
	void backup();
	void undo();
	void clearRows();
	void scored() {score+=100;}
};

#endif