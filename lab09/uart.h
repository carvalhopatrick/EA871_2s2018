/*
 * Roteiro 08
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * 			uart.h
 *
 */
void init_UART0(void);
void putchar_UART0(char);
char getchar_UART0(void);
void puts_UART0(char *);
void init_UART0_interr();

void clear_terminal_UART0(void);
void setpos_terminal_UART0(char, char);
