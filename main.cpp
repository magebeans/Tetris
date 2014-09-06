#include "piece.h"
#include "board.h"
#include "graphics.h"
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>
#include <ctime>

using namespace std;

const int d = 100;

int drop (Board &b, Piece &piece, Point &pos);
void save (Board &b);
void load (Board &b);
void welcome();
int random (int);

int main() {
    int gd = DETECT, gm, i, placed;
    initwindow ( 800 , 640 , "Tetris" );
INIT:
    char c;
    Point pos;
    Piece *pieces = Piece::getPieces();
    Piece *piece;
    Board b;
    srand (time (NULL) );
    clearviewport();
    welcome();
    if (getch() == 'l') {
        load (b);
    }
    clearviewport();
    do {
START:
        piece = pieces + random (7);
        for (i = 0; i < random (10); i++) {
            piece = piece -> getRotation();
        }
        pos = Point (4, 19);
        do {
            while (!kbhit() ) {
                pos.y -= 1;
                delay (d);
                if (b.place (*piece, pos) == PLACE_BAD) {
                    b.undo();
                    if (drop (b, *piece, pos) ) {
                        goto START;
                    } else {
                        goto INIT;
                    }
                }
                b.display();
                b.undo();
            }
            c = getch();
            switch (c) {
                case 'd': pos.x++; break;
                case 'a': pos.x--; break;
                case 'w': piece = piece -> getRotation(); break;
                case 's': if (drop (b, *piece, pos) ) {
                        goto START;
                    } else {
                        goto INIT;
                    }
                case 'e': exit (0);
                case 'q': save (b); break;
                default: break;
            }
            placed = b.place (*piece, pos);
            if (placed == PLACE_OUT_BOUNDS) {
                if (pos.x < 0) {
                    pos.x = 0;
                } else {
                    pos.x = 10 - piece->getWidth();
                }
            }
            b.undo();
            while (b.place (*piece, pos) == PLACE_BAD) {
                b.undo();
                if (c == 'a') {
                    pos.x++;
                } else if (c == 'd') {
                    pos.x--;
                }
            }
            b.undo();
        } while (1);
    } while (1);
}

int drop (Board &b, Piece &piece, Point &pos) {
    pos.y  = b.drop (piece, pos);
    int placed = b.place (piece, pos);
    char go[] = "GAME OVER!";
    char ex[] = "Press `Esc' to exit, anything else to continue";
    char c;
    if (placed == PLACE_ROW_FILLED) {
        b.scored();
        b.clearRows();
    } else if (placed == PLACE_FULL) {
        b.display();
        clearviewport();
        settextstyle (TRIPLEX_FONT, HORIZ_DIR, 8);
        outtextxy ( (getmaxx() - textwidth (go) ) / 2,
                    (getmaxy() - 2 * textheight (go) ) / 2,
                    go);
        settextstyle (GOTHIC_FONT, HORIZ_DIR, 2);
        outtextxy ( (getmaxx() - textwidth (ex) ) / 2,
                    (getmaxy() + textheight (go) ) / 2 + 5,
                    ex);
        c = getch();
        if (c == 27) {
            exit (0);
        } else {
            return 0;
        }
    }
    b.backup();
    b.display();
    return 1;
}                                                                                                                                                                                                                                       \

void welcome() {
    char tet[] = "TETRIS";
    char ins[14][50] = {"Welcome to tetris. The objective of the game is",
                        "to clear the board of pieces.",
                        "A row is cleared when it is completely filled.",
                        "Pieces appear at random at the",
                        "top of the board. The controls are: ",
                        "a - Move piece left ",
                        "d - Move piece right ",
                        "s - Drop piece ",
                        "w - Change rotation ",
                        "q - Quicksave (overwrites previous saves)",
                        "e - Exit (you will lose all progress)",
                        "Press 'l' if you would like to load a previously",
                        "saved game. Press any other ",
                        "key to start a new game."
                       };
    int i;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, 10);
    outtextxy ( (getmaxx() - textwidth (tet) ) / 2,
                5,
                tet);
    settextstyle (SANS_SERIF_FONT, HORIZ_DIR, 1);
    for (i = 0; i < 14; i++)
        outtextxy ( (getmaxx() - textwidth (ins[i]) ) / 2,
                    (10 * textheight (tet) + i * textheight (ins[0]) + 5),
                    ins[i]);
}

void save (Board &b) {
    fstream f;
    f.open ("saves.dat", ios::out | ios::binary | ios::trunc);
    f.write ( (char *) (&b), sizeof (b) );
    outtextxy (10, 40, "Saved");
}

void load (Board &b) {
    fstream f;
    f.open ("saves.dat", ios::in | ios::binary);
    f.read ( (char *) (&b), sizeof (b) );
}

int random (int a) {
    return rand() % a;
}
