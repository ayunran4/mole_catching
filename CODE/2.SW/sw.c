/*
********************************************************************************
* File        	: SW.C
* By          	: CJY
* 2017.07.27.   : Last modified
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>

#define DDR_SW		DDRD
#define PIN_SW		PIND

#define DDR_LED 	DDRB
#define PORT_LED	PORTB

int main(void)
{
    DDR_SW |= (0<<PIN0)|(0<<PIN1)|(0<<PIN2)|(0<<PIN3);
    DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);

		PORT_LED = 0xFF;

		for (;;)
    {
			if (!(PIN_SW & 0x01))
			{
				PORT_LED = 0xEF;
				_delay_ms(200);
				PORT_LED = 0xFF;
			}
			if (!(PIN_SW & 0x02))
			{
				PORT_LED = 0xDF;
				_delay_ms(200);
				PORT_LED = 0xFF;
			}
			if (!(PIN_SW & 0x04))
			{
				PORT_LED = 0xBF;
				_delay_ms(200);
				PORT_LED = 0xFF;
			}
			if (!(PIN_SW & 0x08))
			{
				PORT_LED = 0x7F;
				_delay_ms(200);
				PORT_LED = 0xFF;
			}
		}
}
