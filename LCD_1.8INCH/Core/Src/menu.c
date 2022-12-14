#include <string.h>

#include "LCD.h"

#include "menu.h"
#include "menu_callbacks.h"
#include <stdio.h>

// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))


menu_t menu1 ;
menu_t menu2 ;
menu_t menu3 ;



menu_t menu1 = { 1,"Zapisuj na SD", &menu2, NULL, NULL, NULL, NULL };
menu_t menu2 = { 2,"Transmituj BT", &menu3, &menu1, NULL, NULL,NULL };
menu_t menu3 = { 3,"Zastrzymaj", NULL, &menu2, NULL, NULL,NULL};


 menu_t *currentPointer = &menu1;
 menu_t *nextPointer = &menu2;
 char * name;
char cursor[] = "cursor";


void menu_refresh() { // lcd_refresh();

	LCD_DisplayString(28,45,strcat(currentPointer->name,cursor),&Font8,RED,GRED);

}
void get_number_pos(){

	HAL_Delay(10);//dopisz funkcje od typa

}
void position(){
	HAL_Delay(10); //dopisz funkcjie ile jest pozycji

}
void menu_next() {
	if(!currentPointer->next)
	{return ;
	}
	else
	{
		currentPointer=currentPointer->next;
	}
	menu_refresh();
}

void menu_prev(void) {

	if(!currentPointer->prev)
	{
		return ;
	}
	else
	{
		currentPointer=currentPointer->prev;
	}
	menu_refresh();
}

void menu_enter(void) {

	if(!currentPointer->menu_function && currentPointer->child)
	{
		currentPointer=currentPointer->child;
	}
	else if(currentPointer->menu_function)
	{
		currentPointer->menu_function(&currentPointer);
	}
	else
	{
		return ;
	}

}



