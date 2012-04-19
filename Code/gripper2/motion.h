#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function

void motion(unsigned char dir)
{
	unsigned char port_restore = 0;
	dir &= 0x0f; // removing upper nibbel as it is not needed
	port_restore = PINA;  // reading the PORTA's original status
	port_restore = port_restore & 0xf0;  // setting lower direction nibbel to 0
	port_restore = port_restore | dir;  // adding lower nibbel for direction command and restoring the PORTA status
	PORTA = port_restore;  // setting the command to the port
}

void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

void forward()
{
	motion(0x06);
}
void backward()
{
	motion(0x09);
}
void left()
{
	motion(0x05);
}
void right()
{
	motion(0x0a);
}
void small_left()
{
	motion(0x04);
}
void small_right()
{
	motion(0x02);
}

void stop()
{
	motion(0x00);
}
