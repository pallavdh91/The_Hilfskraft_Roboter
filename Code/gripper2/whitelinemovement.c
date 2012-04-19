#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function

#include "init.h"
#include "buzzer.h"
#include "led.h"
#include "motion.h"
#include "lcd.c"
#include "adc.h"
#include "signals.h"
#include "rfid.h"
#include "servo.h"
void checkfollow(void)
{
	int temp12=0;
	while(1)
	{
		init_sensor_values();
		while(checkobstacle()==0) {} 		
		if(checkintersection() == 1 )
		{	
			temp12++;
			if(temp12==2){ turn_left(); temp12=0; }
			else follow();
			
		}
		else 
		{
			follow();
		}
		
	}
}

void checkarm(void)
{
	while(1)
	{
		go_down1();
	    grab();
		go_up();
		_delay_ms(1000);
		go_down2();
		release();
	    go_up();
		_delay_ms(1000);
	}
}

void checkstraight()
{
	while(1)
	{
		follow();
	}
}


int move_bot()
{
	init_sensor_values();
	while(checkobstacle()==0) {} 
	buzzer_off();
	if(checkintersection() == 1)
	{	
		//communicate with coordinator
		comintersection();
		stop();
		_delay_ms(100);
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
		}
		while(data == '\0' )
		{
			stop();
			_delay_ms(4000);
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
		if(data == 'c') //collect item data -> i when the bot is trying to pick up an rfid
		{
			data='w';
			lcd_cursor(2,1);
			lcd_string("Collecting");

			go_down1();
			
			
	       	grab();
			
			go_up();
		    
		    
			
			_delay_ms(2000);
			readrfidtag();	
			clearrfid();     
			continue;   
			
		}
		if(data == 'd') //drop_item
		{
			data='w';
			lcd_cursor(2,1);
			lcd_string("Dropping");

			go_down2();
			
			release();
			
	        go_up();
			
			
			_delay_ms(2000);
			senddroppedsig();
			continue;
		}
		
		while(1) 
		{
			if(move_bot() == 1)
			{
				continue;
			}
			else 
			{
				break;
			}
		}
	}
}



int main(void)
{
	init_devices();
	lcd_cursor(1,1);		
	lcd_string("Welcome");
	
	//UDR0 = 'a';
	//checkstraight();

	//checkfollow();
	//checkarm();
	run();
	
	
}
