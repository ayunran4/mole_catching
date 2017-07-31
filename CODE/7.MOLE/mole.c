/*
********************************************************************************
* File        	: MOLE.C
* By          	: CJY
* 2017.07.31.   : Last modified
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>   // rand()

#define DDR_SW		DDRD
#define PIN_SW		PIND

#define DDR_LED		DDRB
#define PORT_LED	PORTB

#define DDR_FND 	DDRF
#define PORT_FND	PORTF

#define _PF0 0   // VR
#define _PF1 1   // CDS

#define LED0  4
#define LED1  5
#define LED2  6
#define LED3  7

// Read ADC data
unsigned int get_adc_data(unsigned char port)
{
    unsigned int adc;

    // Select ADC port
    ADMUX |= (0<<REFS1)|(1<<REFS0)|port;

    // Start ADC Conversion
    ADCSRA |= (1<<ADSC);

    // Wait until ADC conversion completed
    while (!(ADCSRA & (1<<ADIF)));

    adc = (int)ADCL+((int)ADCH<<8);
    if (!port)
      adc = (int)(adc/1023.0*9);

    return adc;
}

// ADC Init
void init_adc(void)
{
    // AVCC with external capacitor on AREF pin
    ADMUX |= (0<<REFS1)|(1<<REFS0);
    // PF1 - CDS
    ADMUX |= (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0);
    // Enable ADC, Division factor = 128 (= 125kHz) : 50kHz < 125kHz < 200kHz
    ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// Get Random Number
unsigned char get_random_num()
{
  // Return 4,5,6,7
  return rand()%4+4;
}

int main(void)
{
  unsigned char random, LED_on;
  unsigned char fnd_num[10] = {0x0F,0x1F,0x2F,0x3F,0x4F,0x5F,0x6F,0x7F,0x8F,0x9F};

  DDR_SW |= (0<<PIN0)|(0<<PIN1)|(0<<PIN2)|(0<<PIN3);
  DDR_FND |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
  DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);

  init_adc();

  PORT_LED = 0xFF;

	for (;;)
  {
    random = get_random_num();
    PORT_FND = fnd_num[random];
    switch (random)
    {
      case LED0 : LED_on = 0x10; break;
      case LED1 : LED_on = 0x20; break;
      case LED2 : LED_on = 0x40; break;
      case LED3 : LED_on = 0x80; break;
    }
    PORT_LED = ~LED_on;
    _delay_ms(400);

    PORT_LED = 0xFF;
    _delay_ms(200);
	}
}
