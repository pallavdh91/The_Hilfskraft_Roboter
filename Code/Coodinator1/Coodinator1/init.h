#include <stdio.h>


const int no_of_dbots = 2;
const int no_of_gbots = 1;
int no_of_objects=2;
Point *object_posn;
const int num_col = 8;
const int num_row = 8;
rfid_obj *obj;

HANDLE hSerial;
DWORD dwBytesRead = 0;
DWORD dwBytesWrote = 0;
char *data;
bot delivery[no_of_dbots];
bot gripper[no_of_gbots];
char lastsigd[no_of_dbots];
char lastsigg[no_of_gbots];
int map[num_row][num_col];


void myinit()
{
	for(int i=0;i<num_row;i++)
	{
		for(int j=0;j<num_col;j++)
		{
			map[i][j]=-1;
		}
	}
	gripper[0].set_bot(0,0,1,0,0); 
	delivery[0].set_bot(1,2,2,1,2);
	delivery[1].set_bot(0,3,1,0,3);
	object_posn = new Point[no_of_objects];
	object_posn[0].x = 0;
	object_posn[0].y = 1;
	lastsigg[0]='w';
	lastsigd[0]='w';
	object_posn[1].x = 1;
	object_posn[1].y = 0;
}


void init(char name[])
{
     hSerial = CreateFile(name,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
     if(hSerial==INVALID_HANDLE_VALUE)
     {
             if(GetLastError()==ERROR_FILE_NOT_FOUND)
             {
					printf("could not open port\n");
                    //serial port does not exist. Inform user.
             }
     }
   
    DCB dcbSerialParams = {0};
    DCB dcbSerial;
    dcbSerial.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) 
    {
        //error getting state
    }
    dcbSerialParams.BaudRate=9600;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams))
    {
        //error setting serial port state
        printf("error sending data");
    }
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts))
    {
                                 printf("timeout error\n");
    }
	myinit();
}