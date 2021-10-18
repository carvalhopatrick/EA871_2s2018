

#include <stdio.h>
#include <stdlib.h>

#include "derivative.h"
#include "delay.h"
#include "uart.h"
#include "lcd.h"

#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))




int main(void)
{
	char c_char;
	char *c_hex_str;
	char *c_dec_str;
    
    c_dec_str = malloc(sizeof(char*) * 4);
    c_dec_str[1] = '\0';
    
    c_hex_str = malloc(sizeof(char*) * 4);
    c_dec_str[1] = '\0';
	
	init_UART0();
	init_GPIO_LCD();
	init_LCD();
	
	// Limpa o terminal e posiciona o cursor no canto superior direito.
	putchar_UART0(27);
	putchar_UART0('[');
	putchar_UART0('H');
	putchar_UART0(27);
	putchar_UART0('[');
	putchar_UART0('2');
	putchar_UART0('J');
	
	puts_UART0("Bem-vind@\n\rDigite caracteres: ");
		
	for( ; ; )
	{
		// Se UART0 receber um novo caractere do terminal
		if (UART0_S1 & UART0_S1_RDRF_MASK)
		{
			// Lê o caractere recebido
			c_char = getchar_UART0();
			
			if (c_char == 27)
			{
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('H');
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('2');
				putchar_UART0('J');
				clear_LCD();
				break;
			}
			else
			{
				putchar_UART0(c_char);
				
				// Limpa LCD e coloca novamente "char: "
				clear_LCD();
				setpos_LCD(1,0);
				puts_LCD("char: ");
				
				// Coloca caractere no LCD
				setpos_LCD(1,6);
				putchar_LCD(c_char);
				
				// Coloca valor hexadecimal no LCD
				setpos_LCD(1,8);
				sprintf(c_hex_str, "0x%x", c_char);
				puts_LCD(c_hex_str);
				
				// Coloca valor decimal no LCD
				setpos_LCD(1,13);
				sprintf(c_dec_str, "%d", c_char);
				puts_LCD(c_dec_str);
			}
		}
		
	}
}












