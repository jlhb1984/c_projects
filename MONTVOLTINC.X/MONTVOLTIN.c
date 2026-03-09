/*
 * File:   MONTVOLTIN.c
 * Author: jlhb1
 *
 * Created on 7 de marzo de 2026, 04:21 PM
 */

#include <xc.h>
#define F_CPU 1000000UL  // Tell the compiler the chip is running at 1MHz
#include <avr/io.h>      // Standard AVR Input/Output header
#include <util/delay.h>  // Header for delay functions
#include <avr/interrupt.h>

volatile uint16_t adc_result = 0;

ISR(ADC_vect) {
    // ADC reads 10-bit values (0 to 1023)
    // We read the combined ADCL and ADCH registers automatically via 'ADC'
    adc_result = ADC;
    PORTD=adc_result;
    
    // To keep it running, we start the NEXT conversion here
    ADCSRA |= (1 << ADSC);
}

void main(void) {
     // 1. Set Direction: PB0 as Output
    // DDRB is the Data Direction Register for Port B
    DDRB |= (1 << PB0); 
    DDRD |= 0xFF;
    // 1. REFS0 = 1: Use AVCC (5V) as the reference voltage
    ADMUX |= (1 << REFS0);
    // ADMUX default is 0 for channel, so ADC0 (PA0) is already selected
    // 2. Configure ADCSRA (Control and Status Register A)
    // ADEN: Enable ADC
    // ADIE: Enable ADC Interrupt
    // ADPS1 & ADPS0: Set Prescaler to 8 (1MHz / 8 = 125kHz) 
    // The ADC needs a clock between 50kHz and 200kHz for max accuracy.
    ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS1) | (1 << ADPS0);
    // 3. Enable Global Interrupts
    sei();
    // 4. Start the very first conversion
    ADCSRA |= (1 << ADSC);

    while (1) {
        // 2. Toggle PB0
        //PORTB ^= (1 << PB0); // Use XOR to flip the state of the pin
        PORTB=PORTB^(1 << PB0);
        // 3. Wait 500 milliseconds
        _delay_ms(2000);
    }
    return;
}