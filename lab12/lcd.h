/*
 * Roteiro 09
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * 			lcd.h
 *
 */


void init_GPIO_LCD(void);

void putcmd_LCD(unsigned char, unsigned int);

void clear_LCD(void);

void init_LCD(void);

void putchar_LCD(unsigned char);

void setpos_LCD(unsigned int, unsigned int);

void puts_LCD( char *);



