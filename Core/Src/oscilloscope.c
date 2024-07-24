/*
 * oscilloscope.c
 *
 *  Created on: Mar 14, 2023
 *      Author: Wiktor
 */
#include "lcd.h"
#include "oscilloscope.h"
#include <stdio.h>
#include <math.h>

#define _SQRT3 1.73205
#define SAMPLES_PER_AMP 124.12f
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

uint16_t ADC_buffer[900];
uint8_t range_divider=4;
uint16_t zerocurrentreading=0;

void prepareBackground(void){
	UG_FillScreen(C_BLACK);
	UG_FontSelect(FONT_10X16);
	UG_SetForecolor(C_YELLOW);
	UG_SetBackcolor(C_BLACK);
	UG_FontSetTransparency(0);
	UG_DrawLine(0, 120, 320, 120, C_WHITE);

	UG_DrawLine(0, 150, 280, 150, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 180, 280, 180, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 210, 280, 210, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 90, 280, 90, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 60, 280, 60, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 30, 280, 30, C_ANTIQUE_WHITE);
	drawVBaseUnits();
	UG_FillFrame(200, 228, 205, 233, C_RED);
	UG_PutChar('U', 210, 225, C_YELLOW, C_BLACK);
	UG_FillFrame(225, 228, 230, 233, C_GREEN);
	UG_PutChar('V', 235, 225, C_YELLOW, C_BLACK);
	UG_FillFrame(255, 228, 260, 233, C_BLUE);
	UG_PutChar('W', 265, 225, C_YELLOW, C_BLACK);

	UG_PutString(0, 0, "Button - change hor/ver scale");

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, ADC_buffer, 900);
}

void drawVBaseUnits(void){
	if(range_divider==4){range_divider=8;}else{range_divider=4;}
	if(range_divider==4){
		UG_PutString(283, 85, "1A");
		UG_PutString(283, 55, "2A");
		UG_PutString(283, 25, "3A");
		UG_PutString(283, 145, "-1A");
		UG_PutString(283, 175, "-2A");
		UG_PutString(283, 205, "-3A");
	}
	if(range_divider==8){
		UG_PutString(283, 85, "2A");
		UG_PutString(283, 55, "4A");
		UG_PutString(283, 25, "6A");
		UG_PutString(283, 145, "-2A");
		UG_PutString(283, 175, "-4A");
		UG_PutString(283, 205, "-6A");
	}
}

float calculate_electric_angle(float I_U, float I_V){
	float I_alpha=I_U;
	float I_beta=((1/_SQRT3)*I_U)+((2/_SQRT3)*I_V);
	float angle_deg=atan2f(I_beta,I_alpha)*57.2957;
	return angle_deg;
}

void print_avg(){
	int32_t avg_U_sum=0;
	int32_t avg_V_sum=0;
	int32_t avg_W_sum=0;
	for(uint16_t i=0;i<300;i++){
		avg_U_sum+=(int16_t)ADC_buffer[i*3];
		avg_V_sum+=(int16_t)ADC_buffer[(i*3)+1];
		avg_W_sum+=(int16_t)ADC_buffer[(i*3)+2];
	}
	float avg_U=((avg_U_sum/300)-zerocurrentreading)/SAMPLES_PER_AMP;
	float avg_V=((avg_V_sum/300)-zerocurrentreading)/SAMPLES_PER_AMP;
	float avg_W=((avg_W_sum/300)-zerocurrentreading)/SAMPLES_PER_AMP;
	float angle = calculate_electric_angle(avg_U, avg_V);
	char measurement_string[25];
	char measurement_string2[25];
	sprintf(measurement_string,"U:%.2fA V:%.2fA W:%.2fA",avg_U,avg_V, avg_W);
	sprintf(measurement_string2,"Angle: %.1fdeg",angle);
	UG_SetBackcolor(C_DARK_GRAY);
	UG_SetForecolor(C_WHITE);
	UG_PutString(0, 190, measurement_string);
	UG_PutString(0, 205, measurement_string2);
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_YELLOW);
}

void drawBuffer(){
	if(zerocurrentreading==0){
		uint32_t sum=0;
		for(uint16_t i=300;i<350;i++){
			sum+=ADC_buffer[i];
		}
		zerocurrentreading=sum/50;
	}
	UG_FillFrame(0, 17, 280, 225, C_BLACK);
	UG_DrawLine(0, 120, 320, 120, C_WHITE);
	UG_DrawLine(0, 150, 280, 150, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 180, 280, 180, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 210, 280, 210, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 90, 280, 90, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 60, 280, 60, C_ANTIQUE_WHITE);
	UG_DrawLine(0, 30, 280, 30, C_ANTIQUE_WHITE);
	for(int i=30;i<300;i++){
		//draw U pixel
		int16_t vert_position=(((int16_t)ADC_buffer[i*3]-zerocurrentreading)/range_divider)+120;
		vert_position= vert_position>223 ? 223 : vert_position;  //clip out of screen values
		vert_position= vert_position<17 ? 17 : vert_position;
		UG_DrawPixel(i-30, vert_position,C_RED);
		//draw V pixel
		vert_position=(((int16_t)ADC_buffer[(i*3)+1]-zerocurrentreading)/range_divider)+120;
		vert_position= vert_position>223 ? 223 : vert_position;
		vert_position= vert_position<17 ? 17 : vert_position;
		UG_DrawPixel(i-30, vert_position,C_GREEN);
		//draw W pixel
		vert_position=(((int16_t)ADC_buffer[(i*3)+2]-zerocurrentreading)/range_divider)+120;
		vert_position= vert_position>223 ? 223 : vert_position;
		vert_position= vert_position<17 ? 17 : vert_position;
		UG_DrawPixel(i-30, vert_position,C_BLUE);
	}
	//calculate and show time of aquisition of 300 samples
	uint16_t sampling_time=((htim3.Instance->ARR+1)*3)/10;
	char sampling_time_string[15];
	sprintf(sampling_time_string,"Refresh: %d ms ",sampling_time);
	UG_PutString(10, 225, sampling_time_string);
	print_avg();
}
