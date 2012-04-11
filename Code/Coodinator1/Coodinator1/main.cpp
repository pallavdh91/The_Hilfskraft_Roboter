#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <commctrl.h>
#include "structs.h"
#include "init.h"
#include "xbee.h"
#include "functions.h"

int getdestn_rfid(char *rfid) // this function will decide on which dbot to place to object it.
{
	if(strcmp(rfid,"4108") == 0) return 0;
	else if(strcmp(rfid,"410E") == 0) return 1;
	else return -1;
}

Point getdestn_obj()
{
	Point to_return = Point(3,4);
	return to_return;
}



void deliver(char *data) // code for delivery bot
{
	
	char *to_send;
	to_send = new char[4];
	to_send[0] = data[0];
	to_send[1] = data[1];

	int bot_number = data[1] - '0';
	if( lastsigd[bot_number] == 'f' || lastsigd[bot_number] == 'l' || lastsigd[bot_number] == 'r' ) delivery[bot_number].reachedintersection(); //update coordinates
	printf("Delivery bot%d am at (%d,%d) \n",bot_number,delivery[bot_number].p.x,delivery[bot_number].p.y);
	Point destn=delivery[bot_number].destn;
	if(delivery[bot_number].path == NULL) // if path not known 
	{
		delivery[bot_number].findPath();
	}
	if(!destn.myequal(delivery[bot_number].p)) 
	{
		Point *temppath = delivery[bot_number].path;
		printf("Delivery bot%d: Direction : %d, Path -> ",bot_number,delivery[bot_number].dir);
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
			to_send[2] = getdirection(delivery[bot_number].dir,x,y,curx,cury,&delivery[bot_number].dir);
			delivery[bot_number].path++;
		}
		else 
		{
			to_send[2] = 'h';
			printf("Halting .... \n");
		}
		to_send[3]='\0';
		lastsigd[bot_number] = to_send[2];
		sendString(to_send);
		printf("\n");

	}
	else // reached destination
	{
		printf("Delivery bot%d reached Destination\n\n",bot_number);
	}
}

void trigger_dbot(int bot_number)
{
	delivery[bot_number].is_free=false;
	delivery[bot_number].items++;
	int temp=bot_number; // 
	char *a = new char[1];
	itoa(temp,a,10);
	char *to_send = new char[5];
	to_send[0]='d';
	to_send[1] = a[0];
	to_send[2] = 'f';
	to_send[3] = '\0';
	lastsigd[bot_number] = to_send[2];
	delivery[bot_number].destn = getdestn_obj();
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

	if(gripper[bot_number].items == 0)
	{
		gripper[bot_number].destn = object_posn[0]; //Object Destination set at this time
	}
	//printf("Last signal to gripper: %c",lastsigg[bot_number]);
	if( lastsigg[bot_number] == 'r' || lastsigg[bot_number] == 'l' || lastsigg[bot_number] == 'f' ) gripper[bot_number].reachedintersection(); //update coordinates
	printf("Gripper %d am at (%d,%d) \n",bot_number,gripper[bot_number].p.x,gripper[bot_number].p.y);
	Point destn=gripper[bot_number].destn;
	//printf("My destination is (%d,%d) \n", destn.x , destn.y );
	if(gripper[bot_number].path == NULL) // if path not known 
	{
		//printf("Finding path...\n");
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
			gripper[bot_number].path++;
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
		printf("Reached destination\n");
		gripper[bot_number].path = NULL;
		if(gripper[bot_number].items == 0) 
		{
			to_send[2] = 'c';
			gripper[bot_number].items = 1;
		}
		else if(gripper[bot_number].items == 1)
		{
			to_send[2] = 'd';
		}
		lastsigg[bot_number] = to_send[2];
		sendString(to_send);
	}

}

void updateafterdrop(int bot_number)
{
	gripper[bot_number].items = 0;
	object_posn++;
	no_of_objects--;
	gripper[bot_number].is_free = true;
	trigger_dbot(gripper[bot_number].dest_bot);
}

int main()
{
	init("COM3");
	int i=0;	
	int flag=0;
	while(1)
	{	
		flag=0;
		if(no_of_objects==0) 
		{
			flag=1;
			printf("\nAll objects sorted by Gripper\n\n");
		}
		if(flag==1 && allfree())
		{
			printf("\n\nDelivered all objects\n");
			printf("Project successfully done :) \n");
			break;
		}
		if(gripper[0].is_free && flag==0)
		{
			collect_obj(0);
			gripper[0].is_free = false;
		}
		data=new char[20];
		getString();
		printf("I read %s \n",data);
		if(data[0] == 'd') // for delivery bot
		{
			deliver(data);
		}
		else if(data[0] == 'g')
		{
			if(data[3] == '\0')	collect_obj(0);
			else if(data[3] == '*') //object dropped successfully by gripper
			{
				updateafterdrop(0);
			}
			else
			{
				char* temp = new char;
				temp = data+3;
				printf("Received rfid: %s\n",temp);
				int i = getdestn_rfid(temp);
				gripper[0].dest_bot = i;
				gripper[0].destn = delivery[i].p;
				collect_obj(0);
			}
		}
		else printf("Error signal\n");
		
	}
}