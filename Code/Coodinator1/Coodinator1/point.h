#include <stdio.h>

class Point
{
	public:
	int x;
	int y;
	
	Point()
	{
		x=0;
		y=0;
	}
	Point(int a,int b)
	{
		x=a;
		y=b;
	}
	bool myequal(Point a)
	{
		return(x == a.x && y == a.y);
	}
};