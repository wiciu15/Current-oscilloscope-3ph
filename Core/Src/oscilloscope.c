/*
 * oscilloscope.c
 *
 *  Created on: Mar 14, 2023
 *      Author: Wiktor
 */
#include "lcd.h"
#include "oscilloscope.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

uint16_t ADC_buffer[900];

void prepareBackground(void){
	UG_FillScreen(C_BLACK);
	UG_FontSelect(FONT_10X16);
	UG_SetForecolor(C_YELLOW);
	UG_SetBackcolor(C_BLACK);
	UG_FontSetTransparency(0);
	UG_PutString(10, 10, "U: ");
	UG_PutString(90, 10, "V: ");
	UG_PutString(180, 10, "W: ");
	UG_DrawLine(0, 120, 320, 120, C_WHITE);

	UG_DrawLine(0, 150, 280, 150, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 180, 280, 180, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 210, 280, 210, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 90, 280, 90, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 60, 280, 60, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 30, 280, 30, C_ANTIQUE_WHITE);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, ADC_buffer, 900);
}

void drawBuffer(){
	UG_FillScreen(C_BLACK);
	UG_DrawLine(0, 150, 280, 150, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 180, 280, 180, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 210, 280, 210, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 90, 280, 90, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 60, 280, 60, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 30, 280, 30, C_ANTIQUE_WHITE);
	for(int i=0;i<300;i++){
		UG_DrawPixel(i, (((int16_t)ADC_buffer[i*3]-2950)/4)+120,C_RED);
		UG_DrawPixel(i, (((int16_t)ADC_buffer[(i*3)+1]-2950)/4)+120,C_GREEN);
		UG_DrawPixel(i, (((int16_t)ADC_buffer[(i*3)+2]-2950)/4)+120,C_BLUE);
	}
}
