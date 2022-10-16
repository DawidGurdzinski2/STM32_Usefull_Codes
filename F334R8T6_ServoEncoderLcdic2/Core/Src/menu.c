#include <string.h>

#include "lcd_i2c.h"


#include "menu.h"

#include <stdio.h>

// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))
menu_t menu1 = { "Servo1", &menu2, &menu5, &sub_menu1_1, NULL, NULL };
	menu_t sub_menu1_1 = { "ruchserwem", NULL, NULL, NULL, &menu1, NULL };
menu_t menu2 = { "Servo2", &menu3, &menu1, &sub_menu2_1, NULL, NULL };
	menu_t sub_menu2_1 = { "ruchserwem", &sub_menu2_2, &sub_menu2_4, NULL, &menu2, NULL };
	menu_t sub_menu2_2 = { "nic1", &sub_menu2_3, &sub_menu2_1, NULL, &menu2, NULL };
	menu_t sub_menu2_3 = { "nic2", &sub_menu2_4, &sub_menu2_2, NULL, &menu2, NULL };
	menu_t sub_menu2_4 = { "nic3", &sub_menu2_1, &sub_menu2_3, NULL, &menu2, NULL };

menu_t menu3 = { "dioda", &menu4, &menu2, NULL, NULL,NULL};
menu_t menu4 = { "DUZE NIC1", &menu5, &menu3, NULL, NULL, NULL };
menu_t menu5 = { "DUZE NIC2", &menu1, &menu4, NULL, NULL, NULL };

menu_t *currentPointer = &menu1;
menu_t *nextPointer = &menu2;
uint8_t menu_index=0;






void menu_refresh(struct lcd_disp * lcd) { // lcd_refresh();

	sprintf((char*)(lcd->f_line),currentPointer->name);
	if(!currentPointer->next)
	{	sprintf((char*)(lcd->s_line)," ");}
	else
	{	sprintf((char*)(lcd->s_line),nextPointer->name);}
	change_cursor(lcd,4);
	lcd_display(lcd);
	}





void menu_next(void) {
	if(!currentPointer->next)
	{
		return ;
	}
	else
	{	menu_index++;
		currentPointer=currentPointer->next;
		nextPointer=currentPointer->next;
	}

}

void menu_prev(void) {

	if(!currentPointer->prev)
	{
		return ;
	}
	else
	{	menu_index++;
		nextPointer=currentPointer;
		currentPointer=currentPointer->prev;

	}


}

void menu_enter(void) {

	if(!currentPointer->child)
	{
		return ;
	}
	else
	{
		currentPointer=currentPointer->child;

	}


}

void menu_back(void) {
	if(!currentPointer->parent)
	{
		return ;
	}
	else
	{
		currentPointer=currentPointer->parent;

	}

}