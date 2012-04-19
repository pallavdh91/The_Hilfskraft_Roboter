#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function

unsigned char rfidval[20];
int rfid_count=0;
/*SIGNAL(SIG_USART3_RECV) 		// ISR for receive complete interrupt
{
	cli();

	lcd_cursor(2,1);		
	lcd_string("Reading...");	
	int i;
	for(i=0;i<10;i++)
	{
		lcd_cursor(1,1);		
		lcd_string(rfidval);
		while ( !(UCSR3A & (1<<RXC3)) )
		{	
			
		}
		rfidval[i] = UDR3;
		
	}
	lcd_cursor(1,1);		
	lcd_string(rfidval);
	lcd_cursor(2,1);		
	lcd_string("Inter Done");
	sei();
}
*/
SIGNAL(SIG_USART3_RECV) 		// ISR for receive complete interrupt
{
	cli();
	char mytemp = UDR3;
	lcd_cursor(2,1);		
	lcd_string("Reading...");	
	if(rfid_count==2)
	{
		if(mytemp !=  '0')
		{
			rfidval[rfid_count] = '0';
			rfid_count++;
			rfidval[rfid_count] = mytemp;
		}
		else
		{
			 rfidval[rfid_count] = mytemp;
		}
	}
	else 
	{
		rfidval[rfid_count] = mytemp;	
	}
	rfid_count ++;
	if(rfid_count == 4)
	{
		lcd_cursor(2,1);		
		lcd_string("RfidRead");
		rfidval[4] = '\0';
	}
	sei();
}

void clearrfid()
{
	rfidval[0]='\0';
	rfidval[1]='\0';
	rfidval[2]='\0';
	rfidval[3]='\0';
}

//protocol : dbot :#g srcid/destid.data#
void readrfidtag()
{
	char str[25];
	str[0] = 'g';
	str[1] = myid;
	str[2] = '.';
	str[3] = rfidval[0];
	str[4] = rfidval[1];
	str[5] = rfidval[2];
	str[6] = rfidval[3];
	str[7] = '\0';
	rfid_count=0;
	xbee_sendString(str,7);
}
