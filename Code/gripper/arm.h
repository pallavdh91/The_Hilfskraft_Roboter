#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function

void arm_motion(unsigned char dir)
{
	unsigned char port_restore = 0;
	dir &= 0xf0; // removing lower nibbel as it is not needed
	//port_restore = PINA;  // reading the PORTA's original status
	//port_restore = port_restore & 0x0f;  // setting lower direction nibbel to 0
	port_restore = dir;  // adding lower nibbel for direction command and restoring the PORTA status
	PORTA = port_restore;  // setting the command to the port
}


void arm_down()
{
	arm_motion(0x80);
}

void arm_up()
{
	arm_motion(0x40);
}

void hold()
{
	arm_motion(0x10);
}

void leave()
{
	arm_motion(0x20);
}


