#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function


unsigned char data;
unsigned char chkid1 = '\0';
unsigned char chkid2 = '\0';
unsigned char mytemp;
const char myid = '0'; // 0 is bot 1 and 1 is bot 2
int recv_count=0;
SIGNAL(SIG_USART_RECV) 		// ISR for receive complete interrupt
{
	//receiving protocol : d myid data --> 3chars sent by coordinator
	cli();
	mytemp = UDR ;
	if(recv_count==0) { chkid1=mytemp; recv_count++; }
	else if (recv_count==1) { chkid2=mytemp; recv_count++; }
	else if(recv_count==2)
	{
		if(chkid1 == 'd' && chkid2 == myid)
		{
			data=mytemp;
		}
		chkid1='\0';
		chkid2='\0';
		recv_count=0;
	}
	
	sei();
}

void xbee_sendString(char str[],int strlngt)
{	
	cli();
	int i;
	UDR='#';
	_delay_ms(50);
	for(i=0;i<strlngt;i++)
	{
		UDR=str[i];
		_delay_ms(50);
	}
	UDR='#';
	_delay_ms(50);
	sei();
}

//protocol : dbot :#d srcid/destid.data#
void comintersection()
{
	char str[4];
	str[0] = 'd';
	str[1] = myid;
	str[2] = '.';
	str[3] = '\0';
	xbee_sendString(str,3);
}
