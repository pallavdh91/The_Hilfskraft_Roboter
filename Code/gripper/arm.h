#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>
#include <math.h> //included to support power function

void arm_motion(unsigned char dir)
{
	unsigned char port_restore = 0;
	dir &= 0xf0; // removing lower nibbel as it is not needed
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

void stop_arm()
{
	arm_motion(0x00);
}

void go_up()
{
	arm_up();
    _delay_ms(2750);
    stop_arm();
    _delay_ms(2000);
}

void go_down()
{
	arm_down();
	_delay_ms(2450);
	stop_arm();
	_delay_ms(2000);
}

void grab()
{
	 hold();
    _delay_ms(1600);
    stop_arm();
    _delay_ms(2000);
}

void release()
{
	leave();
	_delay_ms(1600);
	stop();
	_delay_ms(1000);
        
}
