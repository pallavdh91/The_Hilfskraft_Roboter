#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function


unsigned char data;
unsigned char chkid1 = '\0';
unsigned char chkid2 = '\0';
unsigned char mytemp;
const char myid = '1';
SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	//receiving protocol : d myid data --> 3chars sent by coordinator
	cli();
	mytemp = UDR0 ;
	if(chkid1 != '\0') //2nd char received
	{
		if(chkid2 != '\0') //3rd char received
		{
			if(chkid1 != 'g')
			{
				data = '\0';
			}
			else
			{
				if(chkid2 != myid)
				{
					data = '\0';					
				}
				else
				{
					data = mytemp;
					lcd_cursor(1,1);		
					lcd_string(data);
				}
			}
			chkid1='\0';
			chkid2='\0';
		}
		else //setting 2nd char
		{
			chkid2 = mytemp;
		}
		
	}
	else //setting 1st char
	{ 
		chkid1 = mytemp;
	}
	sei();
}


void xbee_sendString(char str[])
{	
	cli();
	int i;
	UDR0='#';
	_delay_ms(1);
	for(i=0;i<strlen(str);i++)
	{
		UDR0=str[i];
		_delay_ms(1);
	}
	UDR0='#';
	_delay_ms(1);
	sei();
}

//protocol : gbot :#g srcid/destid.data#
void comintersection()
{
	char str[10];
	str[0] = 'g';
	str[1] = myid;
	str[2] = '.';
	str[3] = '\0';
	xbee_sendString(str);
}
