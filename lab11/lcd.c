/*  
	EA871 Q - Roteiro 11
	
	Patrick Carvalho
	
*/

#include "lcd.h"
#include "delay.h"
#include "derivative.h"

#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg

void init_GPIO_LCD(){
	SET_BIT(SIM_SCGC5, 11);		   	// Habilita clock do PORTC (System Clock Gating Control
	SET_BIT(SIM_SCGC5, 13);		   	// Habilita clock do PORTC (System Clock Gating Control
	
	
	PORTC_PCR0 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC0
	PORTC_PCR1 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC1
	PORTC_PCR2 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC2
	PORTC_PCR3 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC3
	PORTC_PCR4 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC4
	PORTC_PCR5 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC5
	PORTC_PCR6 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC6
	PORTC_PCR7 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC7
	PORTC_PCR8 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC8
	PORTC_PCR9 = PORTC_PCR0 & 0xFFFFF8FF;	// Zera bits de MUX de PTC9
	
	SET_BIT(PORTC_PCR0, 8);				// Seta bit 0 do MUX de PTC0, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR1, 8);				// Seta bit 0 do MUX de PTC1, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR2, 8);				// Seta bit 0 do MUX de PTC2, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR3, 8);				// Seta bit 0 do MUX de PTC3, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR4, 8);				// Seta bit 0 do MUX de PTC4, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR5, 8);				// Seta bit 0 do MUX de PTC5, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR6, 8);				// Seta bit 0 do MUX de PTC6, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR7, 8);				// Seta bit 0 do MUX de PTC7, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR8, 8);				// Seta bit 0 do MUX de PTC8, assim os bits de MUX serao 001
	SET_BIT(PORTC_PCR9, 8);				// Seta bit 0 do MUX de PTC9, assim os bits de MUX serao 001
	
	GPIOC_PDDR = 0x000003FF;            // Seta bits de 0 a 9 como saída
	GPIOC_PCOR = 0x000003FF;            // inicializa em 0 os bits relevantes do registrador de entrada
}

void init_LCD()
{
	putcmd_LCD(0x0000003C, 41); // faz function set, 2 linhas e display-on
	
	putcmd_LCD(0x0000000F, 41); // faz display on/off
		
	clear_LCD(); // faz clear
	
	putcmd_LCD(0x00000006, 50); // faz entry mode set, com entire shift off
}

void clear_LCD()
{
	putcmd_LCD(0x00000001, 2000);
}

void putchar_LCD(char c)
{
	GPIOC_PSOR = 0x00000100; // R/S = 1 para passagem de dado
	GPIOC_PSOR = 0x00000200; // ativa o enable
	GPIOC_PDOR = (GPIOC_PDOR & 0xffffff00) | c; //não altera R/S nem Enable ao passar o parametro
	GPIOC_PCOR = 0x00000200; // desativa enable
	delay(45);
}

void putcmd_LCD(unsigned char c, unsigned int t)
{
	GPIOC_PCOR = 0x00000100; // R/S = 0 para passagem de comando
	GPIOC_PSOR = 0x00000200; //define Enable = 1
	GPIOC_PDOR = GPIOC_PDOR & 0xffffff00; //não altera R/S nem Enable ao passar o parametro
	GPIOC_PDOR = GPIOC_PDOR | c; //coloca comando respectivo no registrador
	GPIOC_PCOR = 0x00000200; //desativa o enable
	delay(t);
}

void puts_LCD(char *c)
{
	
	int cont;
	
	for (cont = 0; (cont < 16) && (c[cont] != '\0') ; cont++) { 
		
		putchar_LCD( c[cont] ); //coloca os primeiros 16 caracteres na linha 1
		//delay(200); //para facilitar visualização
	}
	
	if ( c[cont] != '\0') {
	
		setpos_LCD(2,0);
	
		for (cont = 16; (cont < 32)  && (c[cont] != '\0' ); cont++) {
		
			putchar_LCD( c[cont] ); //coloca os próximos 16 caracteres na linha 2, 
			//delay(200); //para facilitar visualização
		}
	

		for (cont = 0; (cont < 100)  && (c[cont] != '\0') ; cont++) {
		
			setpos_LCD(1,(16 + cont)); //retorna cursor para a posição logo após a última da primeira linha, sempre atualizada
			putchar_LCD( c[cont+16] ); //coloca ali o primeiro caracter da segunda linha
	
			setpos_LCD(2,(16+cont)); //avança cursor para a posição logo após a última da segunda linha, sempre atualizada
			putchar_LCD( c[cont+32] ); //coloca ali o próximo caracter a ser mostrado 
	
			putcmd_LCD(0x00000018, 400000); //shifta para a esquerda, com delay alto para facilitar visualização
	
		}
		
	}
	
	return;
}

void setpos_LCD(unsigned int i, unsigned int x) //numero da linha e posicao na linha
{
	if (i == 1) {
		
		GPIOC_PCOR = 0x00000100; // R/S = 0 para passagem de comando
		GPIOC_PSOR = 0x00000200; //Enable
		GPIOC_PDOR = GPIOC_PDOR & 0xffffff00; //não altera R/S nem Enable
		GPIOC_PDOR =(GPIOC_PDOR | 0x00000080) | x; // Set DD Ram adress, para reposicionar o cursor dea cordo com parametro X
		GPIOC_PCOR = 0x0000200; //Disable 
		delay(45);
	}
	
	else {
		
		GPIOC_PCOR = 0x00000100; // R/S = 0 para passagem de comando
		GPIOC_PSOR = 0x00000200; //Enable
		GPIOC_PDOR = GPIOC_PDOR & 0xffffff00; //não altera R/S nem Enable
		GPIOC_PDOR = (GPIOC_PDOR | 0x000000c0) | x; // Set DD Ram adress, para reposicionar o cursor
		GPIOC_PCOR = 0x0000200; //Disable 
		delay(45);
	}
}

