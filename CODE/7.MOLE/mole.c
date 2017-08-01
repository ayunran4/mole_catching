/*
********************************************************************************
* File        	: MOLE.C
* By          	: CJY
* 2017.08.01   : Last modified
********************************************************************************
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>   // rand()

// #define DDR_SW		DDRD
// #define PIN_SW		PIND

#define DDR_LED		DDRB
#define PORT_LED	PORTB

#define DDR_FND 	DDRF
#define PORT_FND	PORTF

#define CPU_CLOCK 16000000
#define TICKS_PER_SEC 1000

#define ADC_VR 0    // VR
#define ADC_CDS 1   // CDS

#define LED0  4
#define LED1  5
#define LED2  6
#define LED3  7

volatile unsigned char cnt;
volatile unsigned char start;
volatile unsigned char led_status;

// 8-bit Timer/Counter0 Overflow Interrupt
ISR(TIMER0_OVF_vect)
{
    tic_time++;

    TCNT0 = 256 - (CPU_CLOCK / TICKS_PER_SEC / 64);
}

// INT0 : External Interrupt
ISR(INT0_vect)
{
  if (led_status)
    cnt++;

  if (cnt > 9)
  {
    cnt = 0;
    start = 0;
  }
}

// INT1 : External Interrupt
ISR(INT1_vect)
{
  if (led_status)
    cnt++;

  if (cnt > 9)
  {
    cnt = 0;
    start = 0;
  }
}

// INT2 : External Interrupt
ISR(INT2_vect)
{
  if (led_status)
    cnt++;

  if (cnt > 9)
  {
    cnt = 0;
    start = 0;
  }
}

// INT3 : External Interrupt
ISR(INT3_vect)
{
  if (led_status)
    cnt++;

  if (cnt > 9)
  {
    cnt = 0;
    start = 0;
  }
}

// Check LED on
void check_led_on(unsigned int msec)
{
  tic_time = 0;

  while (msec > tic_time)
  {
    led_status = 1;
  }
  led_status = 0;
}

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
    // Use CDS
    if (!port)
      adc = (int)(adc/1023.0*9);
    else
    {
      adc = (int)(adc/1023.0*5);
      switch (adc) {
        case 0: adc = 100;break;
        case 1: adc = 200;break;
        case 2: adc = 300;break;
        case 3: adc = 400;break;
        case 4: adc = 500;break;
      }
    }

    return adc;
}

// Display LED
void display_led_randomly(unsigned int msec)
{
  unsigned char random,led;

  // Return random number in 4,5,6,7
  random = rand()%4+4;

  // Display LED randomly
  switch (random)
  {
    case LED0 : led = 0x10; break;
    case LED1 : led = 0x20; break;
    case LED2 : led = 0x40; break;
    case LED3 : led = 0x80; break;
  }

  PORT_LED = ~led;
  check_led_on(msec);

  PORT_LED = 0xFF;
  _delay_ms(msec);
}

// GPIO Init
void init_gpio(void)
{
  // DDR_SW |= (0<<PIN0)|(0<<PIN1)|(0<<PIN2)|(0<<PIN3);
  DDR_FND |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
  DDR_LED |= (1<<PORT4)|(1<<PORT5)|(1<<PORT6)|(1<<PORT7);
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

void init_trigger(void)
{
  // Falling edge
  EICRA = (1<<ISC01)|(0<<ISC00)|(1<<ISC11)|(0<<ISC10)|(1<<ISC21)|(0<<ISC20)|(1<<ISC31)|(0<<ISC30);
  EIMSK = (1<<INT0)|(1<<INT1)|(1<<INT2)|(1<<INT3);
}

// Timer/Counter Init
void init_timer()
{
  // Waveform Generation Mode : Normal mode, Clock : clk/64 (prescaler)
  TCCR0B = (0<<WGM02)|(0<<CS02)|(1<<CS01)|(1<<CS00);
  TCCR0A = (0<<WGM01)|(0<<WGM00);
  TCNT0 = 256 - (CPU_CLOCK / TICKS_PER_SEC / 64);
  // Timer/Counter0 Overflow Interrupt : Enable
  TIMSK0 = (0<<OCIE0B)|(0<<OCIE0A)|(1<<TOIE0);
}

int main(void)
{
  unsigned char fnd_num[10] = {0x0F,0x1F,0x2F,0x3F,0x4F,0x5F,0x6F,0x7F,0x8F,0x9F};

  // ---- GPIO ----
  init_gpio();

  // ---- Timer/Counter0 ----
  init_timer();

  // ---- External Interrupt ----
  init_trigger();

  // ---- Analog to Digital Converter ----
  init_adc();

  // ---- Global Interrupt : Enable ----
  sei();

  // Turn off all LED
  PORT_LED = 0xFF;

  for (;;)
  {
    PORT_FND = fnd_num[cnt];

    // Set for random without duplication
    if (!start)
    {
      srand(get_adc_data(ADC_CDS));
      start = 1;
    }

    // Get LED speed from VR
    display_led_randomly(get_adc_data(ADC_VR));
	}
}
