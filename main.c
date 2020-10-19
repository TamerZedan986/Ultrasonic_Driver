/*
 * main.c
 *
 *  Created on: Jan 19, 2020
 *      Author: Tamer_Zedan
 */

#include "icu.h"

#include "lcd.h"
#include <util/delay.h>

uint8 count=0;

uint16 dist=0,duration=0;

void U_Sonic_init(void){
	DDRD|=(1<<PD0);
	PORTD&=~(1<<PD0);
	_delay_us(2);
	PORTD|=(1<<PD0);
	_delay_us(15);
	PORTD&=~(1<<PD0);
}
void Measure_Dist(void){
	count++;
	if(count==1){
		TCNT1=0;
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(count==2){
		duration=TCNT1;
		TCNT1=0;
	}
}

int main()
{
	SREG=(1<<7);
	LCD_init();
	LCD_displayString("Distance:");
	LCD_goToRowColumn(0,12);
	Icu_ConfigType icu_config={F_CPU_CLOCK,RISING};
	Icu_setCallBack(Measure_Dist);
	Icu_init(&icu_config);
	U_Sonic_init();
	while(1){
		LCD_goToRowColumn(0,10);
		if(count==2){
			count=0;
			Icu_setEdgeDetectionType(RISING);
			dist=(duration*0.034)/2;
			LCD_intgerToString(dist);
			LCD_displayString("cm");
			U_Sonic_init();
		}

	}
}
