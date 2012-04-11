#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/signal.h>


// Timer 5 initialised in PWM mode for velocity control
// Prescale:64
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:674.988Hz

void  timer5_init(){
	
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionalit to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

void config()
{
	DDRJ = 0xff; //led output
	PORTJ = 0xff;

	DDRC = DDRC | 0x08; //set output for buzzer PC3 
	PORTC = PORTC & 0xf7;

	DDRA = DDRA | 0xff; //movement of motors .. all pins output
	PORTA = PORTA & 0x00 ; // set 0 initially

	DDRL = DDRL | 0x38; // velocity .. Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x38; //PL3 and PL4 pins are for velocity control using PWM.
	PORTE = PORTE | 0x08;

	//ADC pin config:
	DDRF = 0x00; 
 	PORTF = 0x00;
 	DDRK = 0x00;
 	PORTK = 0x00;

}

void port_init()
{
	config();
	lcd_port_config();
}


void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}
void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x47; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
}

void uart3_rfid_init(void)
{
	 UCSR3B = 0x00; //disable while setting baud rate
	 UCSR3A = 0x00;
	 UCSR3C = 0x06;
	 UBRR3L = 0x47; //set baud rate lo
	 UBRR3H = 0x00; //set baud rate hi
	 UCSR3B = 0x98;
}


void init_devices()
{
	cli();
	port_init();
	adc_init();
	timer5_init();
	lcd_set_4bit();
	lcd_init();
	uart0_init();
	uart3_rfid_init();
	sei();
}
