#include <stdio.h>
#include "point.h"
#include "rfid.h"
int myabs(int a)
{
	if(a>0) return a;
	else return -a;
}

class bot
{
	public:
	Point p;
	int dir; // 1: +y, 2: +x, 3: -y, 4:-x
	int items;
	Point *path;
	Point origin;
	Point destn;
	bool is_free;
	int dest_bot; //only used by gripper bots
	
	bot()
	{
		p.x=0;
		p.y=0;
		dir=0;
		items=0;
		path=NULL;
		is_free=true;
	}
	void set_bot(int a,int b,int c,int d,int e)
	{
		p.x=a;
		p.y=b;
		origin.x = a;
		origin.y = b;
		dir=c;
		items=0;
		destn.x = d;
		destn.y = e;
		is_free=true;
		dest_bot=-1;
	}
	void reachedintersection()
	{
		if(dir==1) p.y++;
		if(dir==2) p.x++;
		if(dir==3) p.y--;
		if(dir==4) p.x--;
	}

	void findPath()
	{

		int s_x, s_y, d_x, d_y;
		s_x = p.x; //1
		s_y = p.y; //0
		d_x = destn.x; //0
		d_y = destn.y; //2

		if(destn.myequal(p)) 
		{
			path=NULL;
			return;
		}
		
		
		int disX = destn.x-p.x; 
		int disY = destn.y-p.y;
		path = new Point[myabs(disX) + myabs(disY)];
		
		int count =0;
		if (disX >= 0 && disY >=0)
		{
			//towards first quadrant
			if (dir == 2 || dir == 3 )
			{
				//move horizontally first
				for (int iter=s_x+1;iter<=d_x;iter++)
				{
					path[count]=Point(iter,s_y);
					count++;
				}
				for (int iter=s_y+1;iter<=d_y;iter++)
				{
					path[count]=Point(d_x,iter);
					count++;
				}
			}
			else if(dir == 1 || dir == 4 ) //towards first quadrant
			{
				//move vertically first
				for (int iter=s_y+1;iter<=d_y;iter++)
				{
					path[count]=Point(s_x,iter);
					count++;
				}
				for (int iter=s_x+1;iter<=d_x;iter++)
				{
					path[count]=Point(iter,d_y);
					count++;
				}
			}
		}
		else if (disX < 0  && disY >= 0)
		{
			//towards second quadrant
			if (dir == 1 || dir == 2 )
			{
				//move vertically first
				for (int iter=s_y+1;iter<=d_y;iter++)
				{
					path[count]=Point(s_x,iter);
					count++;
				}
				for (int iter=s_x-1;iter>=d_x;iter--)
				{
					path[count]=Point(iter,d_y);
					count++;
				}
			}
			else if(dir == 3 || dir == 4 ) //towards second quadrant
			{
				//move horizontally first
				for (int iter=s_x-1;iter>=d_x;iter--)
				{
					path[count]=Point(iter,s_y);
					count++;
				}
				for (int iter=s_y+1;iter<=d_y;iter++)
				{
					path[count]=Point(d_x,iter);
					count++;
				}
			}
		}
		else if (disX < 0 && disY < 0)
		{
			//towards third quadrant (2,1) -> (0,0) 
			if (dir == 2 || dir == 3 )
			{
				//move vertically first
				for (int iter=s_y-1;iter>=d_y;iter--)
				{
					path[count]=Point(s_x,iter);
					count++;
				}
				for (int iter=s_x-1;iter>=d_x;iter--)
				{
					path[count]=Point(iter,d_y);
					count++;
				}
			}
			else if(dir == 1 || dir == 4 ) 
			{
				//move horizontally first
				for (int iter=s_x-1;iter>=d_x;iter--)
				{
					path[count]=Point(iter,s_y);
					count++;
				}
				for (int iter=s_y-1;iter>=d_y;iter--)
				{
					path[count]=Point(d_x,iter);
					count++;
				}
			}
		}
		else if (disX >= 0 && disY < 0)
		{
			//towards fourth quadrant
			if (dir == 3 || dir == 4 )
			{
				//move vertically first
				for (int iter=s_y-1;iter>=d_y;iter--)
				{
					path[count]=Point(s_x,iter);
					count++;
				}
				for (int iter=s_x+1;iter<=d_x;iter++)
				{
					path[count]=Point(iter,d_y);
					count++;
				}
			}
			else if(dir == 1 || dir == 2 ) 
			{
				//move horizontally first
				for (int iter=s_x+1;iter<=d_x;iter++)
				{
					path[count]=Point(iter,s_y);
					count++;
				}
				for (int iter=s_y-1;iter>=d_y;iter--)
				{
					path[count]=Point(d_x,iter);
					count++;
				}
			}
		}				
	}
};