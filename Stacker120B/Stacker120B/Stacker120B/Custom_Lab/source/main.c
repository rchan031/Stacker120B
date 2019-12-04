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
unsigned char game = 0x01;
static unsigned char column_val = 0x80; //vertical
static unsigned char column_sel = 0x1F; //horizontal
static unsigned char mover_floor = 0x1F;
static unsigned char mover_level = 0x80;
static unsigned char temp1 = 0x00;
static unsigned char temp2 = 0x00;
static unsigned char temp3 = 0x00;
static unsigned char temp4 = 0x00;
static unsigned char temp5 = 0x00;
static unsigned char temp6 = 0x00;
static unsigned char temp7 = 0x00;
static unsigned char temp8 = 0x00;
static unsigned char temp9 = 0x00;
static unsigned char temp10 = 0x00;
static unsigned char temp11 = 0x00;
static unsigned char temp12 = 0x00;
static unsigned char temp13 = 0x00;
static unsigned char temp14 = 0x00;
static unsigned char temp15 = 0x00;
static unsigned char temp16 = 0x00;
static unsigned char temp17 = 0x00;
static unsigned char temp18 = 0x00;
static unsigned char temp19 = 0x00;
static unsigned char temp20 = 0x00;
unsigned int score = 0;
unsigned char loseFlag = 0;
unsigned char winFlag = 0;
unsigned char buttonFlag = 0;
unsigned char firstFlag = 0;
unsigned char flagA = 0;
unsigned char flagB = 0;
unsigned char flagC = 0;
unsigned char flagD = 0;
unsigned char flagE = 0;
unsigned char flagF = 0;
unsigned char flagG = 0;
unsigned char flagH = 0;
unsigned int stringCounter = 0;
enum States {wait, save, update} state;
unsigned char leftFlag = 0x00;

void tickFct() {
	switch(state) {
		case wait:
			if((~PIND & 0x01) == 0x01) {
				state = save;
			}
			break;
		case save:
			state = update;
			break;
		case update:
			state = wait;
			break;
		default:
			state = wait;
			break;
	}
	switch(state) {
		case wait:
			if((mover_floor | 0xFE) == 0xFE)  {
				leftFlag = 0x01;
			}
			else if((mover_floor | 0x7F) == 0x7F) {
				leftFlag = 0x00;
			}
			if(leftFlag == 0x00) {
				mover_floor = (mover_floor >> 1) | 0x80;
			}
			else if(leftFlag == 0x01) {
				mover_floor = (mover_floor << 1) | 0x01;
			}
			temp1 = mover_floor;
			temp2 = mover_level;
			break;
		case save:
			firstFlag = 0x01;
			temp3 = mover_floor;
			temp4 = mover_level;
			
			if(mover_level == 0x80){
				score += 1;
				flagA = 1;
			}
			else if(mover_level == 0x40){
				score += 1;
				flagB = 1;
			}
			else if(mover_level == 0x20){
				score += 1;
				flagC = 1;
			}
			else if(mover_level == 0x10){
				score += 1;
				flagD = 1;
			}
			else if(mover_level == 0x08){
				score += 1;
				flagE = 1;
			}
			else if(mover_level == 0x04){
				score += 1;
				flagF = 1;
			}
			else if(mover_level == 0x02){
				score += 1;
				flagG = 1;
			}
			else if(mover_level == 0x01){
				score += 1;
				flagH = 1;
			}
			mover_level = (mover_level >> 1);
			break;
		case update:
			LCD_Cursor(8);
			LCD_WriteData(score + 48);
			break;
		default:
			break;
	}
}

int main(void) {
	unsigned char Character1[8] = {0x00, 0x00, 0x0A, 0x04, 0x02, 0x04, 0x11, 0x0E };
	unsigned int counter = 0;
	unsigned char sm_counter = 0x01;
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFE; PORTD = 0x01;
	firstFlag = 0x01;
	
	LCD_init();
	LCD_Custom_Char(1, Character1);
	
	LCD_WriteCommand(0x80);		/*cursor at home position */
	LCD_DisplayString(1, "Welcome!");
	LCD_WriteCommand(0xc0);
	LCD_WriteData(1);

	TimerSet(1);
	TimerOn();
	
    while (game) {
		if(firstFlag == 0x01) {
			if(stringCounter >= 3500) {
				firstFlag = 0x00;
				LCD_ClearScreen();
				LCD_DisplayString(1, "Score: ");
				LCD_Cursor(8);
				LCD_WriteData(48);
			}
			else {
				++stringCounter;
			}
		}
		if(counter >= 150) {
			tickFct();
			counter = 0;
		}
		
		if(sm_counter % 9 == 0) {
			PORTA = temp2;
			PORTB = temp1;
		}
		
		else if(sm_counter % 9 == 1){
			if(flagA == 1){
				temp5 = temp3;
				temp6 = temp4;
				//temp5 = (temp5 | temp3);
				flagA = 0;
			}
				PORTA = temp6;
				PORTB = temp5;
		}
		else if(sm_counter % 9 == 2){
			if(flagB == 1){
				temp7 = temp3;
				temp8 = temp4;
				temp7 = (temp7 | temp5);
				mover_floor = mover_floor | temp7;
				flagB = 0;
			}
				PORTA = temp8;
				PORTB = temp7;
		}
		else if(sm_counter % 9 == 3){
			if(flagC == 1){
				temp9 = temp3;
				temp10 = temp4;
				temp9 = (temp9 | temp7);
				mover_floor = mover_floor | temp9;
				flagC = 0;
			}
				PORTA = temp10;
				PORTB = temp9;
		}
		else if(sm_counter % 9 == 4){
			if(flagD == 1){
				temp11 = temp3;
				temp12 = temp4;
				temp11 = (temp11 | temp9);
				mover_floor = mover_floor | temp11;
				flagD = 0;
			}
				PORTA = temp12;
				PORTB = temp11;
		}
		else if(sm_counter % 9 == 5){
			if(flagE == 1){
				temp13 = temp3;
				temp14 = temp4;
				temp13 = (temp13 | temp11);
				mover_floor = mover_floor | temp13;
				flagE = 0;
			}
				PORTA = temp14;
				PORTB = temp13;
		}
		else if(sm_counter % 9 == 6){
			if(flagF == 1){
				temp15 = temp3;
				temp16 = temp4;
				temp15 = (temp15 | temp13);
				mover_floor = mover_floor | temp15;
				flagF = 0;
			}
				PORTA = temp16;
				PORTB = temp15;
		}
		else if(sm_counter % 9 == 7){
			if(flagG == 1){
				temp17 = temp3;
				temp18 = temp4;
				temp17 = (temp17 | temp15);
				mover_floor = mover_floor | temp17;
				flagG = 0;
			}
				PORTA = temp18;
				PORTB = temp17;
		}
		else if(sm_counter % 9 == 8){
			if(flagH == 1){
				temp19 = temp3;
				temp20 = temp4;
				temp19 = (temp19 | temp17);
				mover_floor = mover_floor | temp19;
				flagH = 0;
			}
				PORTA = temp20;
				PORTB = temp19;
		}
		
		if(mover_floor == 0xFF) {
			loseFlag = 0x01;
			while(loseFlag) {
				PORTB = 0x00;
				PORTA = 0xFF;
				LCD_Cursor(8);
				LCD_WriteData(score + 47);
				if((~PIND & 0x01) != 0x01) {
					buttonFlag = 0x01;
				}
				if(((~PIND & 0x01) == 0x01) && (buttonFlag == 0x01)) {
					loseFlag = 0x00;
					PORTB = 0xFF;
					PORTA = 0x00;
					mover_floor = 0x1F;
					mover_level = 0x80;
					buttonFlag = 0x00;
					
					score = 0;
					flagA = 0;
					flagB = 0;
					flagC = 0;
					flagD = 0;
					flagE = 0;
					flagF = 0;
					flagG = 0;
					flagH = 0;
					
					temp1 = 0;
					temp2 = 0;
					temp3 = 0;
					temp4 = 0;
					temp5 = 0;
					temp6 = 0;
					temp7 = 0;
					temp8 = 0;
					temp9 = 0;
					temp10 = 0;
					temp11 = 0;
					temp12 = 0;
					temp13 = 0;
					temp14 = 0;
					temp15 = 0;
					temp16 = 0;
					temp17 = 0;
					temp18 = 0;
					temp19 = 0;
					temp20 = 0;
				}
			}
		}
		
		if(mover_level < 0x01) {
			winFlag = 0x01;
			while(winFlag) {
				PORTB = 0x00;
				PORTA = 0xFF;
				LCD_Cursor(8);
				LCD_WriteData(score + 48);
				if((~PIND & 0x01) != 0x01) {
					buttonFlag = 0x01;
				}
				if(((~PIND & 0x01) == 0x01) && (buttonFlag == 0x01)) {
					winFlag = 0x00;
					PORTB = 0xFF;
					PORTA = 0x00;
					mover_floor = 0x1F;
					mover_level = 0x80;
					buttonFlag = 0x00;
					
					score = 0;
					flagA = 0;
					flagB = 0;
					flagC = 0;
					flagD = 0;
					flagE = 0;
					flagF = 0;
					flagG = 0;
					flagH = 0;
					
					temp1 = 0;
					temp2 = 0;
					temp3 = 0;
					temp4 = 0;
					temp5 = 0;
					temp6 = 0;
					temp7 = 0;
					temp8 = 0;
					temp9 = 0;
					temp10 = 0;
					temp11 = 0;
					temp12 = 0;
					temp13 = 0;
					temp14 = 0;
					temp15 = 0;
					temp16 = 0;
					temp17 = 0;
					temp18 = 0;
					temp19 = 0;
					temp20 = 0;
				}
			}
		}
		++sm_counter;
		++counter;
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
