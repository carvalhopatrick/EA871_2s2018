/* 			EA869 - Roteiro 5
	Patrick Penacho Carvalho	185388


				lcd.h
	Declarações funções de lcd.c 	*/



void init_GPIO_LCD(void);

void putcmd_LCD(unsigned char, unsigned int);

void clear_LCD(void);

void init_LCD(void);

void putchar_LCD(unsigned char);

void setpos_LCD(unsigned int, unsigned int);

void puts_LCD(unsigned char *);



