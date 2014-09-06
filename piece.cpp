#include "graphics.h"
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include "piece.h"

using namespace std;

Piece *Piece::pieces   =  NULL;
int Piece::side        =  18;
int Piece::space       =  1;

void Piece::testDraw() {
    Piece *pieces = Piece::getPieces(), *temp;
    int i, gd = DETECT, gm;
    Point cur (10, 4 * side + space);
    initgraph (&gd, &gm, "C:\\TurboC3\\BGI");
    for (i = 0; i < 4; i++) {
        temp = pieces + i;
        do {
            temp->draw (cur);
            cur.x += (5 * side) + (6 * space);
            temp = temp->getRotation();
        } while (temp != pieces + i);
        cur.x  = 0;
        cur.y += (5 * side) + (6 * space);
    }
    clearviewport();
    cur.y = 4 * side + space;
    for (i = 4; i < 7; i++) {
        temp = pieces + i;
        do {
            temp->draw (cur);
            cur.x += (5 * side) + (6 * space);
            temp = temp->getRotation();
        } while (temp != pieces + i);
        cur.x  = 0;
        cur.y += (5 * side) + (6 * space);
    }
}

void Piece::draw (Point cur) {
    int i;
    char sk[5];
    for (i = 0; i < 4; i++) {
        rectangle (cur.x + body[i].x   * side + space,
                   cur.y - body[i].y   * side - space,
                   cur.x + (body[i].x + 1) *side - space,
                   cur.y - (body[i].y + 1) *side + space);
    }
    for (i = 0; i < width; i++) {
        sk[i] = (char) ( (int) (skirt[i]) + (int) ('0') );
    }
    sk[i] = '\0';
    outtextxy (cur.x + width * side / 2, cur.y + 5 * space, sk);
}

Piece::Piece (char pointlist[9], int c) {
    int i, j;
    height = width = 0;
    next           = NULL;
    color          = c;
    for (i = 0; i < 8; i += 2) {
        body[i / 2] = Point (ctoi (pointlist[i]), ctoi (pointlist[i + 1]) );
    }
    for (i = 0; i < 4; i++) {
        if (body[i].y > height) {
            height = body[i].y;
        }
        if (body[i].x > width) {
            width  = body[i].x;
        }
    }
    height++;
    width++;
    skirt = new int[width];
    for (i = 0; i < width; i++) {
        skirt[i] = 10;
        for (j = 0; j < 4; j++)
            if (body[i].x == i && body[j].y < skirt[i]) {
                skirt[i] = body[j].y;
            }
    }
}

Piece *Piece::getPieces() {
    int i;
    Piece *p;
    p = new Piece[7];
    p[0] = Piece ("00010203", LIGHTGREEN);
    p[1] = Piece ("00010210", LIGHTRED);
    p[2] = Piece ("00101112", YELLOW);
    p[3] = Piece ("00101121", LIGHTCYAN);
    p[4] = Piece ("01111020", LIGHTMAGENTA);
    p[5] = Piece ("00011011", BROWN);
    p[6] = Piece ("00101120", LIGHTGRAY);
    for (i = 0; i < 7; i++) {
        p[i].linkRotations();
    }
    pieces = p;
    return p;
}

void Piece::linkRotations() {
    Piece *temp = this;
    char newpointslist[9];
    int i;
    do {
        for (i = 0; i < 8; i += 2) {
            newpointslist[i]   = (char) ('0' + temp->height - 1 - temp->body[i / 2].y);
            newpointslist[i + 1] = (char) ('0' + temp->body[i / 2].x);
        }
        newpointslist[8] = '\0';
        temp->next = new Piece (newpointslist, color);
        if (equals (temp->next) ) {
            delete temp->next;
            temp->next = this;
        } else if (temp->next == NULL) {
            cout << "Low memory" << endl;
            getch();
            exit (0);
        } else {
            temp = temp->next;
        }
    } while (temp->next != this);
}

int Piece::equals (Piece *p) {
    int i, j, pointmatch = 1;
    for (i = 0; i < 4 && pointmatch; i++) {
        pointmatch = 0;
        for (j = 0; j < 4; j++)
            if (body[i] == p->body[j]) {
                pointmatch = 1;
            }
    }
    return pointmatch;
}

int ctoi (char c) {
    return (int) (c - '0');
}