#include <stdio.h>


class rfid_obj
{
	int id;
	char* rfid;
	int dbotnum;
	Point dest;
	rfid_obj(int a,char* b, int c, Point d)
	{
		id=a;
		strcpy(rfid,b);
		dbotnum=c;
		dest=d;
	}
};