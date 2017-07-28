/*
********************************************************************************
* File        	: EXT_INT.C
* By          	: CJY
* 2017.07.27.   : Last modified
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_SW		DDRD
#define PIN_SW		PIND

#define DDR_LED		DDRB
#define PORT_LED	PORTB

#define DDR_FND 	DDRF
#define PORT_FND	PORTF

volatile unsigned char cnt;

ISR(INT0_vect)
{
  cnt++;
  if (cnt > 9)
    cnt = 0;
}

int main(void)
{
  unsigned char num[10] = {0x0F,0x1F,0x2F,0x3F,0x4F,0x5F,0x6F,0x7F,0x8F,0x9F};

  DDR_SW |= (0<<PIN0)|(0<<PIN1)|(0<<PIN2)|(0<<PIN3);
  DDR_FND |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
  DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);

  EICRA = (1<<ISC01)|(0<<ISC00);  // Rising edge
  EIMSK = (1<<INT0);

  PORT_LED = 0xFF;

  sei();
	for (;;)
  {
    if (!(PIN_SW & 0x01))
		{
      PORT_LED = 0xAF;
      PORT_FND = num[cnt];
      _delay_ms(200);
      PORT_LED = 0xFF;
		}
	}
}
