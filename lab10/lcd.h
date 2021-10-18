/*  
	EA871 Q - Roteiro 10
	
	Patrick Carvalho
	
*/

#ifndef LCD_H_
#define LCD_H_

void init_GPIO_LCD(void);

void init_LCD(void);

void clear_LCD(void);

void putchar_LCD(unsigned char c);

void putcmd_LCD(unsigned char c, unsigned int t);

void puts_LCD(unsigned char *c);

void setpos_LCD(unsigned int i, unsigned int x);

#endif /* LCD_H_ */
