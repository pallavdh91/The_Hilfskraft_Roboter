
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.c"
#include "init.h"
#include "adc.h"
#include "buzzer.h"
#include "motion.h"
#include "signals.h"
	
int move_bot()
{
	init_sensor_values();
	while(checkobstacle()==0) {} 
	if(checkintersection() == 1)
	{	
		//communicate with coordinator
		comintersection();
		stop();
		_delay_ms(1000);
		return 0;
	}
	else 
	{
		follow();
		return 1;
	}
}

void run(void)
{
	data='w';
	while(1)
	{
		while(data == 'w') //waiting for signal
		{
			lcd_cursor(1,1);
			lcd_string("w");
		}
		lcd_cursor(1,1);
		lcd_string(data);
		while(data == '\0' )
		{
			stop();
			_delay_ms(500000);
			comintersection();
			_delay_ms(100);
		}
		if(data=='f') // forward
		{
			data='\0';
		}
		if(data=='r') // right	
		{
			data='\0';
			turn_right();
		}
		if(data=='l') // left
		{
			data='\0';
			turn_left();
		} 
		if(data == 'h') // halt
		{
			data='\0';
			stop();
			_delay_ms(100);
			continue;
		
		}
		if(data == 'o') // origin
		{
			data='w';
			stop();
			_delay_ms(100);
			continue;
		}
		while(move_bot() == 1) { }
	}
}

void checkfollow()
{
	int temp=0;
	while(1)
	{
		init_sensor_values();
		//code to check line following:
	 	while(checkobstacle()==0) 
		{}
	 
		if(checkintersection() == 1)
		{
			temp++;
			if(temp==1 || temp > 7) { turn_left(); }
			if(temp==2 || temp ==3 || temp == 5 || temp==6) { turn_right();  }
			if(temp == 4 || temp==7) { follow(); }
			if(temp ==10 ) temp=0;
		}
		else 
		{
			follow();
		}
	}
}

void checkstraight()
{
	while(1)
	{
		follow();
	}
}

void checkxbee()
{
	while(1)
	{
		
		comintersection();
		_delay_ms(1500);
	}
}

//Main Function
int main(void)
{
 
 init_devices();

 
 //checkstraight();
 run();
 //checkfollow();
 //checkxbee();
}
