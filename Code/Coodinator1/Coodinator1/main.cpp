#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <commctrl.h>
#include "structs.h"
#include "init.h"
#include "xbee.h"
#include "functions.h"

int getdestn(char *rfid) // this function will decide where to place it.
{
	return 0;

}

void deliver(char *data) // code for delivery bot
{
	char *temp;
	temp = new char[5];
	int i=0;
	while(data[i] != '.') { temp[i] = data[i]; i++; }
	temp[i] = '\0'; 
	data=new char[100];

	char *to_send;
	to_send = new char[4];
	to_send[0] = temp[0];

	int bot_number =atoi(temp);
	printf("BOT %d\n",bot_number);
	bot_number--;
	if( lastsigd[bot_number] != 'h') delivery[bot_number].reachedintersection(); //update coordinates
	printf("I am at (%d,%d) \n",delivery[bot_number].p.x,delivery[bot_number].p.y);
	Point destn=delivery[bot_number].destn;
	if(delivery[bot_number].path == NULL) // if path not known 
	{
		delivery[bot_number].findPath();
	}
	if(!destn.myequal(delivery[bot_number].p)) 
	{
		Point *temppath = delivery[bot_number].path;
		printf("Direction : %d, Path -> ",delivery[bot_number].dir);
		int abc = myabs(delivery[bot_number].p.x - destn.x) + myabs(delivery[bot_number].p.y - destn.y);
		for(int i=0;i<abc;i++)
		{
			printf("( %d , %d)", temppath[0].x, temppath[0].y);
			temppath++;
		}
		printf("\n");
		
		 // map protocol: -1:free , 0: blocked
		Point next = delivery[bot_number].path[0];
		int x = next.x;
		int y = next.y;

		int curx = delivery[bot_number].p.x;
		int cury = delivery[bot_number].p.y;
		if(map[x][y] == -1)
		{
			map[curx][cury] = -1;
			map[x][y] = 0;
			to_send[1] = getdirection(delivery[bot_number].dir,x,y,curx,cury,&delivery[bot_number].dir);
			delivery[bot_number].path++;
		}
		else 
		{
			to_send[1] = 'h';
			printf("Halting .... \n");
		}
		to_send[2]='\0';
		lastsigd[bot_number] = to_send[1];
		sendString(to_send);
		printf("\n");

	}
	else // reached destination
	{
		printf("Reached Destination\n\n");
	}
}

void trigger_dbot(int bot_number)
{
	delivery[bot_number].is_free=false;
	int temp=bot_number+1; // 
	char *a = new char[1];
	itoa(temp,a,10);
	char *to_send = new char[5];
	to_send[0]='d';
	to_send[1] = a[0];
	to_send[2] = 'h';
	to_send[3] = '\0';
	lastsigg[bot_number] = to_send[2];
	sendString(to_send);
}


void collect_obj(int bot_number) 
{

	char *a = new char[1];
	itoa(bot_number,a,10);
	char *to_send = new char[5];
	to_send[0]='g';
	to_send[1] = a[0];
	to_send[3] = '\0';

	if(gripper[bot_number].items == 0) gripper[bot_number].destn = object_posn[0]; //Object Destination set at this time
	if( lastsigg[bot_number] != 'h' || lastsigg[bot_number] != 'w') gripper[bot_number].reachedintersection(); //update coordinates
	printf("I am at (%d,%d) \n",gripper[bot_number].p.x,gripper[bot_number].p.y);
	Point destn=gripper[bot_number].destn;
	if(gripper[bot_number].path == NULL) // if path not known 
	{
		gripper[bot_number].findPath();
	}
	if(!destn.myequal(gripper[bot_number].p)) 
	{
		//for printing:
		Point *temppath = gripper[bot_number].path;
		printf("Direction : %d, Path -> ",gripper[bot_number].dir);
		int abc = myabs(gripper[bot_number].p.x - destn.x) + myabs(gripper[bot_number].p.y - destn.y);
		for(int i=0;i<abc;i++)
		{
			printf("( %d , %d)", temppath[0].x, temppath[0].y);
			temppath++;
		}
		printf("\n");
		//printing done

		 // map protocol: -1:free , 0: blocked
		Point next = gripper[bot_number].path[0];
		int x = next.x;
		int y = next.y;

		int curx = gripper[bot_number].p.x;
		int cury = gripper[bot_number].p.y;
		if(map[x][y] == -1) // check if it is free!
		{
			map[curx][cury] = -1;
			map[x][y] = 0;
			to_send[2] = getdirection(gripper[bot_number].dir,x,y,curx,cury,&gripper[bot_number].dir);
			delivery[bot_number].path++;
		}
		else 
		{
			to_send[2] = 'h';
			printf("Halting .... \n");
		}
		lastsigg[bot_number] = to_send[2];
		sendString(to_send);
		printf("\n");
	}
	else
	{
		printf("Reached destination");
		if(gripper[bot_number].items == 0) 
		{
			to_send[2] = 'c';
			gripper[bot_number].items = 1;
		}
		else if(gripper[bot_number].items == 1)
		{
			to_send[2] = 'd';
			gripper[bot_number].items = 0;
			trigger_dbot(gripper[bot_number].dest_bot);
		}
		lastsigg[bot_number] = to_send[2];
		sendString(to_send);
	}

}

int main()
{
	init("COM3");
	int i=0;	
	if(gripper[0].is_free)
	{
		collect_obj(0);
		gripper[0].is_free = false;
	}
	while(1)
	{	
		if(no_of_objects == 0 && allfree())
		{
			printf("Delivered all objects\n");
			break;
		}
		data=new char[20];
		getString();
		printf("I read %s \n",data);
		if(data[0] == 'd') // for delivery bot
		{
			data++;
			deliver(data);
		}
		else if(data[0] == 'g')
		{
			if(data[3] == '\0')	collect_obj(0);
			else
			{
				char* temp = new char;
				temp = data+3;
				int i = getdestn(temp);
				gripper[0].dest_bot = i;
				gripper[0].destn = delivery[i].p;
				collect_obj(0);
			}
		}
		else printf("Error signal\n");
		
	}
}