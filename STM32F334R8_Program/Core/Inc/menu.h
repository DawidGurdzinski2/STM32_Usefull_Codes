#ifndef MENU_H_
#define MENU_H_

typedef struct menu_struct menu_t;

struct menu_struct {
	//const uint8_t back;
	const char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(menu_t*);

};



void menu_refresh(struct lcd_disp *);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);


#endif /* MENU_H_ */
