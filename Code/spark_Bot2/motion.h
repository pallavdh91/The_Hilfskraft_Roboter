#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//Function used for setting motor's direction
void motion (unsigned char Direction)
{
 unsigned char PortBRestore = 0;

 Direction &= 0x0F; 			// removing upper nibbel as it is not needed
 PortBRestore = PORTB; 			// reading the PORTB's original status
 PortBRestore &= 0xF0; 			// setting lower direction nibbel to 0
 PortBRestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTB status
 PORTB = PortBRestore; 			// setting the command to the port
}

void forward (void) //both wheels forward
{
  motion(0x06);
}

void stop (void) //hard stop
{
  motion(0x00);
}

void backward (void)        //both wheels backward
{
  motion(0x09);
}

void left (void)            //Left wheel backward, Right wheel forward
{
  motion(0x05);
}

void right (void)           //Left wheel forward, Right wheel backward
{   
  motion(0x0a);
}

void soft_right(void)
{
	motion(0x02);
}

void soft_left(void)
{
	motion(0x04);
}
//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
 OCR1AH = 0x00;
 OCR1AL = left_motor; 
 OCR1BH = 0x00;
 OCR1BL = right_motor;
}
