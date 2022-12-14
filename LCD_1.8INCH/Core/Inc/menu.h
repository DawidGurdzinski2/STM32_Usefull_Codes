#ifndef MENU_H_
#define MENU_H_

typedef struct menu_struct menu_t;

struct menu_struct {
	uint8_t ;
	const char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(menu_t*);

};



void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void position(void);
void get_number_pos(void);

#endif /* MENU_H_ */
