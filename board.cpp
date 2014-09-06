#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
#include "board.h"

using namespace std;

Board::Board() {
    int i, j;
    for (i = 0; i < 24; i++) {
        widths[i] = 0;
        for (j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }
    for (i = 0; i < 10; i++) {
        heights[i] = 0;
    }
    score = 0;
    offset = Point ( (getmaxx() - 10 * Piece::side) / 2,
                     (getmaxy() - 24 * Piece::side) / 2);
    backup();
}

void Board::backup() {
    int i, j;
    for (i = 0; i < 24; i++)
        for (j = 0; j < 10; j++) {
            xgrid[i][j] = grid[i][j];
        }
    for (i = 0; i < 10; i++) {
        xheights[i] = heights[i];
    }
    for (j = 0; j < 24; j++) {
        xwidths[j] = widths[j];
    }
}

void Board::undo() {
    int i, j;
    for (i = 0; i < 24; i++)
        for (j = 0; j < 10; j++) {
            grid[i][j] = xgrid[i][j];
        }
    for (i = 0; i < 10; i++) {
        heights[i] = xheights[i];
    }
    for (j = 0; j < 24; j++) {
        widths[j] = xwidths[j];
    }
}

int Board::drop (Piece p, Point pos) {
    while (place (p, pos) != PLACE_BAD) {
        undo();
        pos.y--;
    }
    undo();
    return pos.y + 1;
}

int Board::place (Piece p, Point loc) {
    Point pos;
    int i, j;
    if (loc.x < 0 || loc.x + p.getWidth() > 10) {
        return PLACE_OUT_BOUNDS;
    }
    if (loc.y < 0) {
        return PLACE_BAD;
    }
    for (i = 0; i < 4; i++) {
        pos = p.getBody (i);
        pos.x += loc.x;
        pos.y += loc.y;
        if (grid[pos.y][pos.x]) {
            return PLACE_BAD;
        } else {
            grid[pos.y][pos.x] = p.getColor();
        }
    }
    for (i = 0; i < 4; i++) {
        widths[loc.y + p.getBody (i).y]++;
    }
    for (i = loc.y; i < loc.y + p.getHeight(); i++)
        if (widths[i] == 10) {
            return PLACE_ROW_FILLED;
        }
    for (i = 0; i < 4; i++)
        if (heights[loc.x + p.getBody (i).x] < loc.y + p.getBody (i).y + 1) {
            heights[loc.x + p.getBody (i).x] = loc.y + p.getBody (i).y + 1;
        }
    for (i = 0; i < 4; i++)
        if (heights[p.getBody (i).x + loc.x] >= 19) {
            return PLACE_FULL;
        }
    return PLACE_OK;
}

void Board::clearRows() {
    int i, j, k;
    for (i = 0; i < 23; i++) {
        if (widths[i] == 10) {
            for (j = i; j < 23 && widths[j]; j++) {
                for (k = 0; k < 10; k++) {
                    grid[j][k] = grid[j + 1][k];
                }
                widths[j] = widths[j + 1];
            }
            i--;
        }
    }
    for (i = 0; i < 10; i++) {
        for (j = 23; j >= 0 && !grid[j][i]; j--);
        heights[i] = j + 1;
    }
}

void Board::display() {
    int i, j, maxy = getmaxy();
    char sc[10];
    //itoa (score, sc, 10);
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, 3);
    setfillstyle (SOLID_FILL, BLACK);
    bar (10, 10, 5 * textwidth (sc), 1.8 * textheight (sc) );
    outtextxy (10, 10, sc);
    for (i = 0; i < 11; i++)
        line ( offset.x + Piece::side * ( (float) (i) - 0.5),
               maxy - offset.y - Piece::side * ( (float) (24) - 0.5),
               offset.x + Piece::side * ( (float) (i) - 0.5),
               maxy - offset.y + Piece::side * (0.5) );
    for (j = 0; j < 25; j++)
        line ( offset.x + Piece::side * (-0.5),
               maxy - offset.y - Piece::side * ( (float) (j) - 0.5),
               offset.x + Piece::side * ( (float) (10) - 0.5),
               maxy - offset.y - Piece::side * ( (float) (j) - 0.5) );
    for (j = 0; j < 24; j++) {
        for (i = 0; i < 10; i++) {
            if (grid[j][i]) {
                setfillstyle (SOLID_FILL, grid[j][i]);
                floodfill (offset.x + Piece::side * i, maxy - offset.y - Piece::side * j,
                           WHITE);
            } else if (j == xheights[i]) {
                setfillstyle (SLASH_FILL, DARKGRAY);
                floodfill (offset.x + Piece::side * i, maxy - offset.y - Piece::side * j,
                           WHITE);
            } else {
                setfillstyle (SOLID_FILL, BLACK);
                floodfill (offset.x + Piece::side * i, maxy - offset.y - Piece::side * j,
                           WHITE);
            }
        }
    }
}
