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

int main(void) {
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//char i;
	unsigned char Character1[8] = {0x00, 0x00, 0x0A, 0x04, 0x02, 0x04, 0x11, 0x0E };
	LCD_init();
	LCD_Custom_Char(1, Character1);
	
	LCD_WriteCommand(0x80);		/*cursor at home position */
	LCD_DisplayString(1, "Welcome!");
	LCD_WriteCommand(0xc0);
	LCD_WriteData(1);
    while (1) {

    }
    return 1;
}
