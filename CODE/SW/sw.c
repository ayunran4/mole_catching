/*
********************************************************************************
* File        	: LED.C
* By          	: CJY
*
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>

#define DDR_LED		DDRB
#define PORT_LED	PORTB

int main(void)
{
    int i, j;
   
    DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
    PORT_LED = 0xFF;

    for (;;)
    {
        PORT_LED = 0x00;
    	_delay_ms(200);		
        PORT_LED = 0xFF;
    	_delay_ms(200);		
    }
}

