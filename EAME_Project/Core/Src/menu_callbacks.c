
#include <string.h>
#include "common.h"
#include "menu.h"
#include "menu_callbacks.h"
//#include "servo.h"


extern void (*key_next_func)(void);
extern void (*key_prev_func)(void);
extern void (*key_enter_func)(void);

void go_back(menu_t** temp){

	menu_t* currentPointer= *temp;
	*temp =currentPointer->parent;

}

