/*
 * File:   MONTVOLTIN.c
 * Author: jlhb1
 *
 * Created on 7 de marzo de 2026, 04:21 PM
 * PA0  pin40   ADC0    Channel0 to ADC.
 * PD2  pin16   INT0    External interrupt.
 * PD5  pin19   OC1A    Comparator mode output.
 * PB0  pin1            Trigger signal.
 */

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // Essential for ISR and sei()
#include <util/delay.h>

uint8_t low_byte;
uint8_t high_byte;

// 1. Define the ISR (Interrupt Service Routine)
// This code runs AUTOMATICALLY when PD2 has a rising edge.
ISR(INT0_vect) {
    cli();
    ADCSRA|=(1 << ADSC);
    PORTB=0b00000000;
    sei();
}

ISR(ADC_vect){
    cli();
    low_byte=ADCL;
    high_byte=ADCH,
    //PORTB=ADCL;
    OCR1AL=low_byte;
    OCR1AH=high_byte;
    TIMSK|=(1<<OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect){
    cli();    
    //PORTD|=(0<<PD5);
    PORTB=0b00000001;    
    sei();
}

int main(void) {
    DDRA=0b11111110;
    DDRB=0b11111111;    //PB_OUT
    DDRD=0b01110010;    //OC2_IN,ICP1_OUT,OC1A_OUT,OC1B_OUT,INT1_IN,INT0_IN,TXD_OUT,RXD_IN
    MCUCR=0b00000011;   //RISING EDGE
    GICR=0b01000000;    //INT0 ENABLE
    ADCSRA=0b10001000;  //ADEN_1,ADCS_START_0,ADATE_0,ADIF,ADIE_1,ADPS2_0,ADPS1_0,ADPSO_0
    TCCR1A=0b01000000;  //COM1A1_1,COM1A0_1,COM1B1_0,COM1B0_0,FOC1A_0,FOCIB_0,WGM11_0,WGN10_0
    TCCR1B=0b00001001;  //ICNC1_0,ICES1_0,-,WGM13_0,WGM12_1,CS12_0,CS11_0,CS10_1
    sei();    

    while (1) {
        //sei();
        // Your main code lives here. It can be doing anything 
        // (or nothing), and the interrupt will still work!
    }
}