#include "common.h"
#include "menu.h"
#include <stdint.h>
#include "tim.h"


void (*key_next_func)(void) = &menu_next;
void (*key_prev_func)(void) = &menu_prev;
void (*key_enter_func)(void) = &menu_enter;

uint8_t oldcount=0;
uint8_t count=0;


void key_next_press(void){
		count=__HAL_TIM_GET_COUNTER(&htim3)/2;
		if(oldcount<count)
		{	oldcount=count;
			if(key_next_func)
				{
				(*key_next_func)();


				}
		}




}

void key_prev_press(void){

	   count=__HAL_TIM_GET_COUNTER(&htim3)/2;
		if(oldcount>count)
		{	oldcount=count;
			if(key_prev_func) (*key_prev_func)();
		}

}



void key_enter_press(void){

	if(key_enter_func) (*key_enter_func)();


}


