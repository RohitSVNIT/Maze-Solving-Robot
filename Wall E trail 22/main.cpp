/*
 * Wall E trail 21.cpp
 *
 * Created: 19-02-2016 09:09:11 PM               
 * Author : Rohit Kaduru
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
volatile unsigned int input=0,f,l,r;


int main(void)
{
	_delay_ms(1000);
	
	// for ADC
	ADCSRA |= 1<<ADPS2;  // enabling pre-scalar
	ADCSRA |= 1<<ADIE;   // enabling interupt
	ADMUX |= 1<<ADLAR;   // enabling 8-bit
	ADMUX |= 1<<REFS0;   // max reference voltage
	ADCSRA |= 1<<ADEN;   // turning on ADC feature
	sei();
	ADCSRA |= 1<<ADSC;   // ADC Start conversion
	
	// for PWM
	DDRD |= 0xFF;
	DDRB |= 0xFF;
	PORTB |= 0x00;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0 | 1<<COM1B0 | 1<<COM1B1;
	TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10;
	ICR1 = 19999;
	
	
	
	while(1)                                     
	{
		
		
		
		
		
		
		
		
	}
}

ISR(ADC_vect)
{
	input = ADCH ;
	
	switch(ADMUX)
	{
		
		// Front Sensor - First Case
		case 0x60:
		f = input;
		// Code - 1	
		if(f>160)                          // Go Straight
		{
			OCR1A = 14000;
			OCR1B = 14000;
			PORTB |= 1<<PINB0;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB1);
			PORTB &= ~(1<<PINB2);
		}
		if(f<=160 && f>113)              // When near wall
		{
			OCR1A = 16000;
			OCR1B = 16000;
			PORTB |= 1<<PINB0;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB1);
			PORTB &= ~(1<<PINB2);
		}	
		if(f<113)                             // 90 degree Right Turn when front closed
		{
			OCR1A = 5000;
			OCR1B = 5000;
			PORTB |= 1<<PINB1;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB0);
			PORTB &= ~(1<<PINB2);
			_delay_ms(320);
		}
		ADMUX = 0x61;
		break;
		
		// Left Sensor - Second Case
		case 0x61:
		l = input;
		// Code - 2
		if(l<91)
		{
			OCR1A = 3000;
			OCR1B = 17000;
			PORTB |= 1<<PINB0;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB1);
			PORTB &= ~(1<<PINB2);
		}
		if(l>160)                    // 90 degree Slow left When right is available
		{
			OCR1A = 17000;
			OCR1B = 17000;
			PORTB |= 1<<PINB0;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB1);
			PORTB &= ~(1<<PINB2);
			_delay_ms(100);
			OCR1A = 17000;
			OCR1B = 5000;
			PORTB |= 1<<PINB0;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB1);
			PORTB &= ~(1<<PINB2);
			_delay_ms(610);
		}
		ADMUX = 0x62;
		break;
		
		
		// Right Sensor - Third Case
		case 0x62:
		r = input;
		// Code - 3
		if(r<91)
		{
			OCR1A = 17000;
			OCR1B = 3000;
			PORTB |= 1<<PINB0;
			PORTB |= 1<<PINB3;
			PORTB &= ~(1<<PINB1);
			PORTB &= ~(1<<PINB2);
		}
		ADMUX = 0x63;
		break;
		
		
		// Front Sensor - Fourth Case  (change from first place)
		case 0x63:	
		// Code - 4
		ADMUX = 0x60;
		break;
	}
	
	ADCSRA |= 1<<ADSC;
	
}
