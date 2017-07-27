/*
********************************************************************************
* File        	: SW.C
* By          	: CJY
*
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
    //DDR_SW |= (0<<PIN0)|(0<<PIN1)|(0<<PIN2)|(0<<PIN3);
	DDRD = 0x00;
    DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
    
	PORT_LED = 0xFF;

    for (;;)
    {
		if (PIND == 0x01)
		{
			PORT_LED = 0xFE;
			_delay_ms(200);
			PORT_LED = 0xFF;
		}
		/*if (!(PIN_SW & 0x02))
		{
			PORT_LED = (0<<PORT1);
			_delay_ms(200);
		}
		if (!(PIN_SW & 0x04))
		{
			PORT_LED = (0<<PORT2);
			_delay_ms(200);
		}
		if (!(PIN_SW & 0x08))
		{
			PORT_LED = (0<<PORT3);
			_delay_ms(200);
		}*/
    }
}

