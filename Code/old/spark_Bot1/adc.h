#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


unsigned char ADC_Value;
unsigned char lline = 0;
unsigned char cline = 0;
unsigned char rline = 0;
unsigned char fir = 0;

int leftspeed=40;
int rightspeed=40;
int delta=-3; // positive if right is faster .. -65 for bot1 and -5 for bot2

//for PID
float kp=1.2;
float kd=1.22;
float ki=0;
int prop=0;
int der=0;
int integral=0;
int last_prop=0;
int pow_diff;
int max=80; //speed

int thresh = 155;
//int speed=50;
//int delta=0; // positive if right is faster



//This Function accepts the Channel Number and returns the corresponding Analog Value 
unsigned char ADC_Conversion(unsigned char Ch)
{
 unsigned char a;
 Ch = Ch & 0x07;  			
 ADMUX= 0x20| Ch;	   		
 ADCSRA = ADCSRA | 0x40;	//Set start conversion bit
 while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
 a=ADCH;
 ADCSRA = ADCSRA|0x10;      //clear ADIF (ADC Interrupt Flag) by writing 1 to it
 return a;
}

void init_sensor_values(void)
{
	lline = ADC_Conversion(3);	//Getting data of Left WL Sensor
	cline = ADC_Conversion(4);	//Getting data of Center WL Sensor
	rline = ADC_Conversion(5);	//Getting data of Right WL Sensor
	print_sensor(2,1,3);		//Prints value of White Line Sensor Left
	print_sensor(2,5,4);		//Prints value of White Line Sensor Center
	print_sensor(2,9,5);		//Prints Value of White Line Sensor Right
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
	
	fir = ADC_Conversion(1); // front IR
}

// This Function prints the Analog Value Of Corresponding Channel No. at required Row
// and Coloumn Location. 
void print_sensor(char row, char coloumn,unsigned char channel)
{
 ADC_Value = ADC_Conversion(channel);
 lcd_print(row, coloumn, ADC_Value, 3);
}


void take_turn(int d)
{
	int left1;
	int right1;
	int flag=0;
	backward();
	
	if(d==1)
	{
		velocity(leftspeed*1.2,(rightspeed-delta));
		_delay_ms(6000);
	}
	else if(d==2) 
	{
		velocity(0,(rightspeed-delta));
		_delay_ms(6000);
	}
	
	stop();
	_delay_ms(1000);
	if(d==1) soft_left();
	else if(d==2) soft_right();
	while(1)
	{
		init_sensor_values();
		int left1 = leftspeed;
		int right1 = rightspeed-delta;
		if(cline>thresh)
		{
			if(lline>thresh) left1 = left1*0.7;
			else if(rline>thresh) right1 = right1*0.7;
		}
		else
		{
			if(lline>thresh) left1 = left1/2;
			else if(rline>thresh) right1 = right1/2;
		}
		velocity(left1,right1);
		if(lline<thresh && cline>thresh && rline<thresh) break;
	}
	/*while(1)
	{
		if(d==1) left();
		else if(d==2) right();
		init_sensor_values();	
		//if(pow_diff < 0) velocity((max+pow_diff), max);
		//else velocity(max, (max-pow_diff));	
		if(d==1) velocity(leftspeed,(rightspeed-delta));
		else if(d==2) velocity(leftspeed,(rightspeed-delta));
		//left1=speed*0.5;
		//right1=(speed-delta)*0.5;
		//velocity(left1,right1);
		//lcd_print(2,13,left1,2);
		//lcd_print(2,15,right1,2);
		
		
		if(cline<thresh && lline<thresh && rline<thresh)  //all on white
		{
			_delay_ms(130);
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
	*/
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

int checkobstacle() //returns 0 if there is an obstacle
{
	init_sensor_values();
	if(fir<0x28)
	{
		
		stop();
		lcd_cursor(1,1);		
		lcd_string("Obstacle");		
		_delay_ms(100);		
		return  0;
	}	
	return 1;
}



int checkintersection() 	//returns 1 if there is an intersection
{
	init_sensor_values();
	if( cline>thresh && lline>thresh && rline >thresh ) //all on black
	{
		
		forward();
		velocity(leftspeed,(rightspeed-delta));
		_delay_ms(3150);
		lcd_cursor(1,1);		
		lcd_string("Intersection");
		stop();
		_delay_ms(3000);
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
		forward();
		stop();
		lcd_cursor(1,1);		
		lcd_string("Stop");
		
	}
	
}

