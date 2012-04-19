#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function


unsigned char data;
unsigned char chkid1 = '\0';
unsigned char chkid2 = '\0';
unsigned char mytemp;
const char myid = '0';
int recv_count=0;;
SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	//receiving protocol : d myid data --> 3chars sent by coordinator
	cli();
	mytemp = UDR0 ;
	if(recv_count==0) { chkid1=mytemp; recv_count++; }
	else if (recv_count==1) { chkid2=mytemp; recv_count++; }
	else if(recv_count==2)
	{
		if(chkid1 == 'g' && chkid2 == myid)
		{
			data=mytemp;
		}
		chkid1='\0';
		chkid2='\0';
		recv_count=0;
	}
	sei();
	lcd_cursor(1,1);
	char msg[3];
}


void xbee_sendString(char str[],int strlngt)
{	
	cli();
	int i;
	UDR0='#';
	_delay_ms(1);
	for(i=0;i<strlngt;i++)
	{
		UDR0=str[i];
		_delay_ms(1);
	}
	UDR0='#';
	_delay_ms(1);
	sei();
}

void senddroppedsig()
{
	char str[10];
	str[0] = 'g';
	str[1] = myid;
	str[2] = '.';
	str[3] = '*'; //for dropped
	str[4] = '\0';
	xbee_sendString(str,4);
}

//protocol : gbot :#g srcid/destid.data#
void comintersection()
{
	char str[10];
	str[0] = 'g';
	str[1] = myid;
	str[2] = '.';
	str[3] = '\0';
	xbee_sendString(str,3);

}
