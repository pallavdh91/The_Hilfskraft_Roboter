#include <stdio.h>



char getdirection(int dir,int x,int y, int curx, int cury, int *finaldir)
{
	char  to_return = '\0';
	
	
	if(dir == 1) // +y
	{
		if(curx - x == 1) { to_return = 'l'; *finaldir=4; }
		else if(curx - x == -1) { to_return = 'r'; *finaldir=2; }
		else if(cury - y == -1) { to_return = 'f'; *finaldir=1; }
		else if(cury - y == 1) { to_return = 'u';  *finaldir=3; }
		else printf("error");
	}
	else if(dir == 3) // -y
	{
		if(curx - x == 1) { to_return = 'r';  *finaldir=4; }
		else if(curx - x == -1) { to_return = 'l';  *finaldir=2; }
		else if(cury - y == -1) { to_return = 'u';  *finaldir=1; }
		else if(cury - y == 1) { to_return = 'f';  *finaldir=3; }
		else printf("error");
	}
	else if(dir == 2) // +x
	{
		if(curx - x == 1) { to_return = 'u';  *finaldir=4; }
		else if(curx - x == -1) { to_return = 'f';  *finaldir=2; }
		else if(cury - y == -1) { to_return = 'l';  *finaldir=1; }
		else if(cury - y == 1) { to_return = 'r';   *finaldir=3; }
		else printf("error");
	}
	else if(dir == 4) // -x
	{
		if(curx - x == 1) { to_return = 'f';  *finaldir=4; }
		else if(curx - x == -1) { to_return = 'u';  *finaldir=2; }
		else if(cury - y == -1) { to_return = 'r';  *finaldir=1; }
		else if(cury - y == 1) { to_return = 'l';  *finaldir=3; }
		else printf("error");
	}
	return  to_return;
}

bool allfree()
{
	for(int i=0;i<no_of_dbots;i++)
	{
		if(!delivery[i].is_free) return false;
	}
	for(int i=0;i<no_of_gbots;i++)
	{
		if(!gripper[i].is_free) return false;
	}
	return true;
}