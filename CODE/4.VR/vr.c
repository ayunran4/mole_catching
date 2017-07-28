/*
********************************************************************************
* File        	: VR.C
* By          	: CJY
* 2017.07.28.   : Last modified
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>

#define DDR_SW		DDRD
#define PIN_SW		PIND

#define DDR_LED		DDRB
#define PORT_LED	PORTB

#define DDR_FND 	DDRF
#define PORT_FND	PORTF

// Read ADC data
unsigned int get_adc_data(void)
{
    unsigned int adc;

    // Start ADC Conversion
    ADCSRA |= (1 << ADSC);

    // Wait until ADC conversion completed
    while (!(ADCSRA & (1<<ADIF)));

    adc = (int)ADCL+((int)ADCH<<8);
    adc = (int)(adc/1023.0*9);

    return adc;
}

// ADC Init
void init_adc()
{
    // AVCC with external capacitor on AREF pin
    ADMUX |= (0<<REFS1)|(1<<REFS0);
    // DK-AVR Schematic : PF0 - VR
    ADMUX |= (0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
    // Enable ADC, Division factor = 128 (= 125kHz) : 50kHz < 125kHz < 200kHz
    ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

int main(void)
{
  unsigned int adc_data;
  unsigned char num[10] = {0x0F,0x1F,0x2F,0x3F,0x4F,0x5F,0x6F,0x7F,0x8F,0x9F};

  DDR_SW |= (0<<PIN0)|(0<<PIN1)|(0<<PIN2)|(0<<PIN3);
  DDR_FND |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
  DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);

  init_adc();

  PORT_LED = 0xFF;

	for (;;)
  {
    adc_data = get_adc_data();

    PORT_FND = num[adc_data];
    _delay_ms(200);
	}
}
