#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>


void display_led(unsigned char to_display)
{
	PORTJ = to_display;
}
