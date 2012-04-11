#include <stdio.h>


char getStringTemp()
{
	char name;
	//name = new char[2];
     if(ReadFile(hSerial,&name, 1,&dwBytesRead, NULL)==0)
     {
		printf("unable to read check the port(reading)\n");
     }
	 return name;
}

void getString()
{
     int i=0;
     char temp;
	 while(1)
	 {	
		temp=getStringTemp();
		if(temp=='#') break;
	 }
	 temp=getStringTemp();
	 while(temp!='#')
	 {
		 data[i] = temp;
		 i++;
		 temp = getStringTemp();
	 }
     data[i]='\0';
     return;
}


void sendString(char name[])
{
     if(!WriteFile(hSerial,name, strlen(name), &dwBytesWrote, NULL))
     {
         printf("unable to write check the port(sending)\n");
	 }
	 else
	 {
		printf("Sending data:%s:\n",name);
	 }

}
