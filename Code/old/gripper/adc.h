/************************************************************************************
 
 This experiment demostrates the application of a simple line follower robot. The 
 robot follows a white line over a black backround
 
 Connection Details:  	L-1 PA0		L-2 PA1
   						R-1 PA2		R-2	PA3
   						PWML OC5A	PWMR OC5B
*************************************************************************************/
/********************************************************************************
 Written by: Aditya Sharma NEX Robotics Pvt. Ltd.
 Edited by: Sachitanand Malewar, NEX Robotics Pvt. Ltd.
 AVR Studio Version 4.17, Build 666

 Date: 13th January 2010
 
  Application example: Adaptive Cruise Control (ACC)

 Concepts covered:  ADC, LCD interfacing, motion control based on sensor data

 LCD Connections:
 			  LCD	  Microcontroller Pins
 			  RS  --> PC0
			  RW  --> PC1
			  EN  --> PC2
			  DB7 --> PC7
			  DB6 --> PC6
			  DB5 --> PC5
			  DB4 --> PC4

 ADC Connection:
 			  ACD CH.	PORT	Sensor
			  0			PF0		Battery Voltage
			  1			PF1		White line sensor 3
			  2			PF2		White line sensor 2
			  3			PF3		White line sensor 1
			  4			PF4		IR Proximity analog sensor 1*****
			  5			PF5		IR Proximity analog sensor 2*****
			  6			PF6		IR Proximity analog sensor 3*****
			  7			PF7		IR Proximity analog sensor 4*****
			  8			PK0		IR Proximity analog sensor 5
			  9			PK1		Sharp IR range sensor 1
			  10		PK2		Sharp IR range sensor 2
			  11		PK3		Sharp IR range sensor 3
			  12		PK4		Sharp IR range sensor 4
			  13		PK5		Sharp IR range sensor 5
			  14		PK6		Servo Pod 1
			  15		PK7		Servo Pod 2

 ***** For using Analog IR proximity (1, 2, 3 and 4) sensors short the jumper J2. 
 	   To use JTAG via expansion slot of the microcontroller socket remove these jumpers.  
 
 Motion control Connection:
 			L-1---->PA0;		L-2---->PA1;
   			R-1---->PA2;		R-2---->PA3;
   			PL3 (OC5A) ----> PWM left; 	PL4 (OC5B) ----> PWM right; 
 
 LCD Display interpretation:
 ****************************************************************************
 *LEFT WL SENSOR	CENTER WL SENSOR	RIGHT WL SENSOR		BLANK			*
 *BLANK				BLANK				BLANK				BLANK			*
 ****************************************************************************
 
 Note: 
 
 1. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

 	Microcontroller: atmega2560
 	Frequency: 11059200
 	Optimization: -O0 (For more information read section: Selecting proper optimization options 
						below figure 4.22 in the hardware manual)

 2. Make sure that you copy the lcd.c file in your folder

 3. Distance calculation is for Sharp GP2D12 (10cm-80cm) IR Range sensor

*********************************************************************************/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function

#define FCPU 11059200ul //defined here to make sure that program works properly

unsigned char ADC_Value;
unsigned char lline = 0;
unsigned char cline = 0;
unsigned char rline = 0;
unsigned char fsharp=0;
unsigned char fir=0;

int leftspeed=120; // 140 for bot 1 , 105 for bot2
int rightspeed = 120;
int delta = -65; // positive if right is faster .. -65 for bot1 and -5 for bot2

float kp=1;
float kd=0;
float ki=0;
float prop=0;
float der=0;
float integral=0;
float last_prop=0;
float pow_diff;
int max=200; //speed
int thresh = 150;





//Function For ADC Conversion
unsigned char ADC_Conversion(unsigned char Ch) 
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;  			
	ADMUX= 0x20| Ch;	   		
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

// This Function prints the Analog Value Of Corresponding Channel No. at required Row
// and Coloumn Location. 
void print_sensor(char row, char coloumn,unsigned char channel)
{
 ADC_Value = ADC_Conversion(channel);
 lcd_print(row, coloumn, ADC_Value, 3);
}


void init_sensor_values(void)
{
	lline = ADC_Conversion(3);	//Getting data of Left WL Sensor
	cline = ADC_Conversion(2);	//Getting data of Center WL Sensor
	rline = ADC_Conversion(1);	//Getting data of Right WL Sensor
	char str[4];
	str[0] = lline;
	str[1] = cline;
	str[2] = rline;
	str[3] = ',';
	//xbee_sendString(str,4);
	print_sensor(2,1,3);		//Prints value of White Line Sensor Left
	print_sensor(2,5,2);		//Prints value of White Line Sensor Center
	print_sensor(2,9,1);		//Prints Value of White Line Sensor Right
	/*
	if(lline < thresh )
	{
		if(cline < thresh) prop =max/2; // white white black
		else if(rline>thresh) prop=max/4; //white black black 
	}
	else if(rline < thresh)
	{
		if(lline<thresh) prop=0;  // white black white
		else if(cline<thresh) prop=-max/2; // black white white
		else prop=-max/4; // black black white
	}
	der = prop-last_prop; //derivative
	integral += prop; // integral
	last_prop = prop; 
	pow_diff = kp*prop + ki*integral + kd*der;
	if(pow_diff > max) pow_diff = max;
	if(pow_diff < -max) pow_diff = -max;
	*/
	fsharp = ADC_Conversion(11);
	fir = ADC_Conversion(6);
}


void take_turn(int d)
{
	int flag=0;
	while(1)
	{
		if(d==1) left();
		else if(d==2) right();
		init_sensor_values();
		//if(pow_diff < 0) velocity((max+pow_diff), max);
		//else velocity(max, (max-pow_diff));	
		if(d==1) velocity(leftspeed,(rightspeed-delta));
		else if(d==2) velocity(leftspeed,(rightspeed-delta));

		if(cline<thresh && lline<thresh && rline<thresh)  //all on white
		{
			stop();
			_delay_ms(50);
			break;
		}
	}
	while(1)
	{
		if(d==1) left();
		else if(d==2) right();
		init_sensor_values();
		//if(pow_diff < 0) velocity((max+pow_diff)/2, max/2);
		//else velocity(max/2, (max-pow_diff)/2);
		if(d==1) velocity(leftspeed*0.5,(rightspeed-delta));
		else if(d==2) velocity(leftspeed,(rightspeed-delta)*0.5);

		if(cline>thresh && lline>thresh*0.6 && d==1) flag=1;
		if(cline>thresh && rline>thresh*0.6 && d==2) flag=1;
		if(flag==1)
		{
			stop();
			_delay_ms(40);
			break;
		}
	}

}

void turn_left()
{
	lcd_cursor(1,1);		
	lcd_string("Left");
	take_turn(1);
}
void turn_right()
{
	lcd_cursor(1,1);		
	lcd_string("Right");
	take_turn(2);
}

int checkobstacle()
{
	init_sensor_values();
	if(fsharp>0x82 || fir<0xF0)
	{
		stop();
		lcd_cursor(1,1);		
		lcd_string("Obstacle");		
		_delay_ms(100);		
		return  0;
	}	
	return 1;
}

int checkintersection()
{
	init_sensor_values();
	_delay_ms(1);
	if( cline>thresh && lline>thresh && rline >thresh)
	{
		buzzer_on();
		lcd_cursor(1,1);		
		lcd_string("Intersection");
		init_sensor_values();
		forward();
		velocity(leftspeed,(rightspeed-delta));
		_delay_ms(200);		
		stop();
		_delay_ms(1000);
		buzzer_off();
		return 1;
	}
	return 0;
}


void follow()
{
	lcd_cursor(1,1);		
	lcd_string("Go Straight");
	init_sensor_values();
	forward();
	//if(pow_diff < 0) velocity(max+pow_diff, max);
	//else velocity(max, max-pow_diff);
	int left = leftspeed;
	int right = rightspeed-delta;
	if(cline>thresh)
	{
		if(lline>thresh) left = left*0.7;
		else if(rline>thresh) right = right*0.7;
	}
	else
	{
		if(lline>thresh) left = left/2;
		else if(rline>thresh) right = right/2;
	}
	velocity(left,right);
	if((cline<thresh) && (lline<thresh) && (rline<thresh) ) 
	{
		
		stop();
		_delay_ms(100);
		lcd_cursor(1,1);		
		lcd_string("Stop");
		
	}
}


