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

enum States {wait, increment} state;
unsigned char leftFlag = 0x00;

void tickFct() {
	static unsigned char column_val = 0x80; //vertical
	static unsigned char column_sel = 0x1F; //horizontal
	static unsigned char block = 0x00;
	switch(state) {
		case wait:
			if((~PIND & 0x01) == 0x01) {
				state = increment;
			}
			break;
		case increment:
			state = wait;
			break;
	}
	switch(state) {
		case wait:
			if(column_sel == 0xF8) {
				leftFlag = 0x01;
			}
			else if(column_sel == 0x1F) {
				leftFlag = 0x00;
			}
			
			if(leftFlag == 0x00) {
				column_sel = (column_sel >> 1) | 0x80;
			}
			else if(leftFlag == 0x01) {
				column_sel = (column_sel << 1) | 0x01;
			}
			break;
		case increment:
			block = column_sel;
			column_val = (column_val >> 1);
			break;
		default:
			state = wait;
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
	
	
	TimerSet(350);
	TimerOn();
	
    while (1) {
		tickFct();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
