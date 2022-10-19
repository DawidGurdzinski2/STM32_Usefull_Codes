#include <string.h>

#include "lcd_i2c.h"

#include "menu.h"
#include "menu_callbacks.h"
#include <stdio.h>

// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))

menu_t sub_menu1_1;
menu_t menu1 ;
menu_t menu2 ;
	menu_t sub_menu2_1;
	menu_t sub_menu2_2 ;
	menu_t sub_menu2_3 ;
	menu_t sub_menu2_4 ;

menu_t menu3 ;
menu_t menu4 ;
menu_t menu5;



menu_t menu1 = { "Servo1", &menu2, &menu5, &sub_menu1_1, NULL, NULL };
	menu_t sub_menu1_1 = { "goback", NULL, NULL, NULL, &menu1, go_back };
menu_t menu2 = { "Servo2", &menu3, &menu1, &sub_menu2_1, NULL,NULL };
	menu_t sub_menu2_1 = {  "ruchserwem", &sub_menu2_2, &sub_menu2_4, NULL, &menu2,NULL};
	menu_t sub_menu2_2 = { "nic1", &sub_menu2_3, &sub_menu2_1, NULL, &menu2, NULL };
	menu_t sub_menu2_3 = { "nic2", &sub_menu2_4, &sub_menu2_2, NULL, &menu2, NULL };
	menu_t sub_menu2_4 = { "goback", &sub_menu2_1, &sub_menu2_3, NULL, &menu2, go_back };

menu_t menu3 = { "dioda", &menu4, &menu2, NULL, NULL,NULL};
menu_t menu4 = { "ruchserweCW", &menu5, &menu3, NULL, NULL, NULL };
menu_t menu5 = { "DUZE NIC2", &menu1, &menu4, NULL, NULL, NULL };


 menu_t *currentPointer = &menu1;
 menu_t *nextPointer = &menu2;
 char * name;
 //zmiana wskazników zadzialala

//1.volatile - nie działa
//2. wylaczyc optymalizacje - nie działą
//3. zakres widocznosci zmiennych

void menu_refresh(struct lcd_disp * lcd) { // lcd_refresh();
	sprintf((char*)(lcd->f_line),currentPointer->name);
	if(!currentPointer->next)
	{	sprintf((char*)(lcd->s_line)," ");}
	else
	{
		nextPointer=currentPointer->next;
		sprintf((char*)(lcd->s_line),nextPointer->name);}
	lcd_display(lcd);
	}

void menu_next() {
	if(!currentPointer->next)
	{return ;
	}
	else
	{
		currentPointer=currentPointer->next;
	}

}

void menu_prev(void) {

	if(!currentPointer->prev)
	{	return ;
	}
	else
	{
		currentPointer=currentPointer->prev;
	}
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



