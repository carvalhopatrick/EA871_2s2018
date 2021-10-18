/* 			EA869 - Roteiro 5
	Patrick Penacho Carvalho	185388


				main.c
	Programa mostra no display LCD o alfabeto inteiro,
	realizando o devido deslocamento no display, utilizando
    funções desenvolvidas em lcd.c e delay.c 				]*/


#include "derivative.h" /* include peripheral declarations */
#include "delay.h"     
#include "lcd.h"


int main(void)
{
	init_GPIO_LCD();	// Inicia pinos ligados ao LCD
	init_LCD();			// Setup do display LCD
	
	unsigned char alfabeto[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	puts_LCD(alfabeto); 	// Mostra a string alfabeto no display LCD
	
	return 0;
}
