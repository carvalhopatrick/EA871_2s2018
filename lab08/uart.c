/*
 * Roteiro 08
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * 			uart.c
 *
 */
 
#include "derivative.h" 
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))



 
 
/* Função init_UART
		Inicializa a operação da UART0 (MUX, Clock) para que a comunicação seja feita 
		a uma taxa de 19200 bps e com pacotes de informação compostos de 8 bits de dados, 1 stop bit 
		e sem bit de paridade.                      														*/
void init_UART0(void)
{
	SET_BIT(SIM_SCGC4, 10);	// Habilita clock de UART0
	SET_BIT(SIM_SCGC5, 9);	// Habilita clock do PORTA (para PTA1 = RX e PTA2 = TX)
	
	// Seta os MUX de PTA1 e PTA2 para 010 (Função Alt 2 - RX e TX de UART0)
	PORTA_PCR1 &= 0xFFFFF8FF;		
	PORTA_PCR2 &= 0xFFFFF8FF;
    SET_BIT(PORTA_PCR1, 9);
	SET_BIT(PORTA_PCR2, 9);	
 
	// Seleciona o sinal de clock para UART0 usando o SIM_SOPT2 (clock = MCGFLLCLK)
	SIM_SOPT2 &= 0xF3FFFFFF;	// Reseta bits 27 e 26 
	SET_BIT(SIM_SOPT2, 26);		// Seta 26
    
    // Zera PTA1 e PTA2 para inicio
    GPIOA_PDOR &= 0xFFFFFFF9;
 
    // Configura OSR (oversampling ratio) de UART0 para 14x (13+1)
    UART0_C4 = 0b00001101;

    // Configura o UART0_BDH e UART0_BDL (ambos 8 bits):
	//		Sem interrupções (usar polling), stop = 1 bit, BR = 13
	//				baud rate  =  (baud clock) / ((OSR+1) * BR)
	// 		no caso, baud clock = 20.97 MHz (MCGFLLCLK), OSR+1 = 14
	//		resulta num baud rate de ~115200 bits/s (115220 calculado)
	UART0_BDH = 0x00;
	UART0_BDL = 0x0D;

	UART0_C2 |= 0x0c;   // Habilita TX e RX para UART0
}
 
 
/* Função putchar_UART0
		envia um caractere pela UART0	*/
void putchar_UART0(char carac)
{
	// Espera até flag TDRE = 1 (Transmiter Data Register Empty),
	// 		ou seja, até interface estiver desocupada para enviar novo dado.
	while( !(UART0_S1 & UART0_S1_TDRE_MASK) ) ;
	UART0_D = carac;		// envia caractere para UART0
}
 
/* Função getchar_UART0
		Recebe um caractere pela UART0	*/
char getchar_UART0(void)
{
	char carac;
	// Checa se flag RDRF = 1 (Receive Data Register Full),
	//		ou seja, se há dado recebido na UART0.
	if (UART0_S1 & UART0_S1_RDRF_MASK)
	{
		carac = UART0_D; // lê o caractere
        return carac;
	}
    else
    {
        return '\0';
    }
}
 
/* Função puts_UART0
		Envia uma string pela UART0	*/
void puts_UART0(char *p){
	
	int i; 
	for (i=0; p[i] != '\0'; i++)
    {        
		putchar_UART0(p[i]);
    }
}



/* Funçao clear_terminal_UART0
  		Envia a ASCII Escape Sequence para limpar o terminal.	*/
void clear_terminal_UART0(void)
{
	putchar_UART0(27);
	putchar_UART0('[');
	putchar_UART0('H');
	putchar_UART0(27);
	putchar_UART0('[');
	putchar_UART0('2');
	putchar_UART0('J');
}

/* Funçao setpos_terminal_UART0
  		Envia a ASCII Escape Sequence para limpar ir até
  		linha e coluna especificada (range de 0-99 - apenas 2 dig.)	*/
void setpos_terminal_UART0(int linha, int coluna)
{

	
	putchar_UART0(27);
	putchar_UART0('[');
    
	putchar_UART0(48 + (linha/10));
    putchar_UART0( 48 + (linha - ((linha/10)*10)) );
        
	putchar_UART0(';');
    
    putchar_UART0(48 + (coluna/10));
    putchar_UART0( 48 + (coluna - ((coluna/10)*10)) );

	putchar_UART0('H');

}
 
void cursorback_terminal_UART0(int qntd)
{
	putchar_UART0(27);
	putchar_UART0('[');
    putchar_UART0(48 + (qntd/10));
    putchar_UART0( 48 + (qntd - ((qntd/10)*10)) );
	putchar_UART0('D');
}
 
 
 
 
 
 
 
 
