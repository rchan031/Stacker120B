/*	Author: rchan031
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States {sm1_display} state;
void tickFct() {
	static unsigned char column_val = 0x80;
	static unsigned char column_sel = 0xE3;
	
	switch(state) {
		case sm1_display:
			break;
		default:
			state = sm1_display;
			break;
	}
	switch(state) {
		case sm1_display:
			if((~PIND & 0x01) && column_val == 0x80) {
				column_val = column_val + 0x40;
			}
			else if((~PIND & 0x01) && column_val == 0xC0) {
				column_val = column_val + 0x20;
			}
			else if((~PIND & 0x01) && column_val == 0xE0) {
				column_val = column_val + 0x10;
			}
			else if((~PIND & 0x01) && column_val == 0xF0) {
				column_val = column_val + 0x08;
			}
			else if((~PIND & 0x01) && column_val == 0xF8) {
				column_val = column_val + 0x04;
			}
			else if((~PIND & 0x01) && column_val == 0xFC) {
				column_val = column_val + 0x02;
			}
			else if((~PIND & 0x01) && column_val == 0xFE) {
				column_val = column_val + 0x01;
			}
			else if((~PIND & 0x01) && column_val == 0xFF) {
				column_val = 0x80;
			}
			break;
		default:
			break;
	}
	PORTA = column_val;
	PORTB = column_sel;
}
int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	//DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x00; PORTD = 0xFF;
	
	TimerSet(100);
	TimerOn();
	
    while (1) {
		tickFct();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
