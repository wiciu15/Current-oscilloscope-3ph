/*
 * oscilloscope.c
 *
 *  Created on: Mar 14, 2023
 *      Author: Wiktor
 */
#include "lcd.h"

void prepareBackground(void){
	UG_FillScreen(C_BLACK);
	UG_FontSelect(FONT_10X16);
	UG_SetForecolor(C_YELLOW);
	UG_SetBackcolor(C_BLACK);
	UG_FontSetTransparency(0);
	UG_PutString(10, 10, "U: ");
	UG_PutString(90, 10, "V: ");
	UG_PutString(180, 10, "W: ");

}
