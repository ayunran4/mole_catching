/*
********************************************************************************
* File        	: LED.C
* By          	: CJY
* 2017.07.26.   : Last modified   
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>

#define DDR_LED		DDRB
#define PORT_LED	PORTB

int main(void)
{   
    DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
    PORT_LED = 0xFF;

    for (;;)
    {
        PORT_LED = 0x00;
    	_delay_ms(200);		
        PORT_LED = 0xFF;
    	_delay_ms(200);		
    }

	return 0;
}

