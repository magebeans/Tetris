#ifndef PIECE_H
#define PIECE_H
class Point{
public:
	int x,y;
	Point(int a=0,int b=0): x(a),y(b) {};

	inline int operator==(const Point &rhs)
	{ return ((x == rhs.x) && (y == rhs.y))?(1):(0); }
};

class Piece{
private:
	Point body[4];
	int color;
	int width;
	int height;
	int* skirt;
	Piece* next;

	void linkRotations();
	int equals(Piece *p);

	static Piece *pieces;
public:
	Piece(char pointlist[9],int c);
	Piece() {};
	static Piece* getPieces();
	int getWidth()				{return width;}
	int getHeight()				{return height;}
	int getSkirt(int i)			{return skirt[i];}
	int getColor()				{return color;}
	Point getBody(int i)		{return body[i];}
	Piece* getRotation()			{return next;}
	void draw(Point cur);
	static void testDraw();
	
	static int side;
	static int space;
};

inline int ctoi(char c);
#endif	

//Changes
//Point cur to Point loc in void draw
//Made all static members private. Affect functionality?