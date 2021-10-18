/*
 * Roteiro 06
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * 			lcd.c
 *
 */

#include "derivative.h" 
#include "delay.h"
#include "lcd.h"

#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))
#define WAIT	800

/* Função init_GPIO_LCD */
void init_GPIO_LCD(void)
{
	SET_BIT(SIM_SCGC5, 11);    // Seta bit 11 de SCGC5, habilitando clock de PORTC
	
	// Seta os MUX de PTC0-PTC9 para 001
	PORTC_PCR0 &= 0xFFFFF8FF;	
	SET_BIT(PORTC_PCR0, 8);		
	PORTC_PCR1 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR1, 8);
	PORTC_PCR2 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR2, 8);
	PORTC_PCR3 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR3, 8);
	PORTC_PCR4 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR4, 8);
	PORTC_PCR5 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR5, 8);
	PORTC_PCR6 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR6, 8);
	PORTC_PCR7 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR7, 8);
	PORTC_PCR8 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR8, 8);
	PORTC_PCR9 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR9, 8);
	
	GPIOC_PDDR = 0x000003FF;	// Configura PTC0-PTC9 como saida
	GPIOC_PDOR = 0x00000000;	// Garante nivel 0 nas saidas de PTC
}

/* Função putcmd_LCD 
		Usar um t de no minimo 250ns */
void putcmd_LCD(unsigned char instr, unsigned int t)
{
	GPIOC_PDOR = 0b1000000000;	// Limpa GPIOC; Enable = 1
	delay(100);
	GPIOC_PDOR = (GPIOC_PDOR | instr);
	delay(t);
	GPIOC_PCOR = 0b1000000000;	// Enable = 0
	delay(300);					// Espera para evitar outro enable em tempo < que tcycE
}

/* Função clear_LCD */
void clear_LCD(void)
{
	putcmd_LCD(0x0001, 2000);	// Coloca instrucao de clear no LCD
}

/* Função init_LCD
 		E  RS D7 D6 D5 D4 D3 D2 D1 D0
		9  8  7  6  5  4  3  2  1  0	*/
void init_LCD(void)
{
	putcmd_LCD(0b00111100, WAIT);	// Function set: DL = 1, N = 1, F = 1
	putcmd_LCD(0b00001111, WAIT);	// ON/OFF control: D = 1, C = 1, B = 1 
	clear_LCD();
	//putcmd_LCD(0b00000111, WAIT);	// Entry Mode SET: I/D = 1, SH = 1
	putcmd_LCD(0b00000110, WAIT);	// Entry Mode SET: I/D = 1, SH = 1
	delay(100000);
}

/* Função putchar_LCD */
void putchar_LCD(unsigned char carac)
{
	GPIOC_PDOR = 0b1100000000;	// Limpa GPIOC; Enable = 1; RS = 1
	delay(100);
	GPIOC_PDOR = (GPIOC_PDOR | carac);
	delay(2000);
	GPIOC_PCOR = 0b1100000000;	// Enable = 0; RS = 0
	delay(300);		
}

/* Função setpos_LCD
		2 linhas (1 e 2)
 		16 caracteres por linha (0-15) 	*/
void setpos_LCD(unsigned int linha, unsigned int coluna)
{
	if (linha == 1)
	{
		putcmd_LCD((0x80 + coluna), WAIT);
	}
	else if(linha == 2)
	{
		putcmd_LCD((0xC0 + coluna), WAIT);
	}
}

/* Função puts_LCD */
void puts_LCD(char *str)
{
	int i = 0;
	
	while(str[i] != '\0')
	{
		putchar_LCD(str[i]);
		i++;
	}
}









