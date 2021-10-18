/*  
	EA871 Q - Roteiro 10
	
	Patrick Carvalho
	
*/

#include "uart.h"
#include "derivative.h"

#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg

void init_UART0() {
	
	SET_BIT(SIM_SCGC4, 10); //Habilita clock do UART0
	SET_BIT(SIM_SCGC5, 9);  //Habilita clock do PORTA
	
	PORTA_PCR1 = PORTA_PCR1 & 0xFFFFF8FF;	// Zera bits de MUX de PTA1
	PORTA_PCR2 = PORTA_PCR2 & 0xFFFFF8FF;	// Zera bits de MUX de PTA2
	
	SET_BIT(PORTA_PCR1, 9); // Seta bit 0 do MUX de PTA1, assim os bits de MUX serao 010
	SET_BIT(PORTA_PCR2, 9); // Seta bit 0 do MUX de PTA2, assim os bits de MUX serao 010
							//Ativamos aqui a Alternative 2, fazendo de PTA1 o UART0_RX e de PTA2 o UART0_TX
	
	SIM_SOPT2 = SIM_SOPT2 & 0xf3ffffff;
	SET_BIT(SIM_SOPT2, 26); //configura sinal do clock
	               
	GPIOA_PDOR = GPIOA_PDOR & 0xfffffff9; //zera PTA1 e PTA2

	UART0_BDH = 0x2; //configura bdh
	UART0_BDL = 0x23; //configura bdl para 2400 bps
	
	UART0_C2 |= 0x0c; //configura o MUX para que os sinais de RX e TX da UART0 fiquem associados aos pinos PTA1 e PTA2
	
}

char getchar_UART0(void) {
	
	char c;
	uint8_t rdrf_MASK = 0x20;
	
	if ( UART0_S1 & rdrf_MASK ) // verifica se existe um novo caractere na UART0 (Register Full)
	{
		c = UART0_D; // lê o caracter
		return c;
	}
	else
	{
		return '\0';
	}
	
}

void puts_UART0(char *p){ //imprimi até o fim da string
	
	int i;
	i = 0; 
	for (;p[i] != '\0'; i++) 
		putchar_UART0(p[i]);
}
