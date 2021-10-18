/*
 * Roteiro 08
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * 			delay.c
 *
 */

#include "derivative.h" 
#include "delay.h"	
#include "lcd.h"

#define BIT18	0x00040000;
#define BIT23	0x800000;
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg


void init_GPIO()
{
	SET_BIT(SIM_SCGC5, 10);		   			// Habilita clock do PORTB (System Clock Gating Control
	PORTB_PCR18 = PORTB_PCR18 & 0xFFFFF8FF;	// Zera bits de MUX de PTB18
	SET_BIT(PORTB_PCR18, 8);				// Seta bit 0 do MUX de PTB18, assim os bits de MUX serao 001
	SET_BIT(GPIOB_PDDR, 18);				// Seta pino 18 do PORTB como saida 
	
	SET_BIT(SIM_SCGC5, 13);		   			// Habilita clock do PORTE (System Clock Gating Control
	PORTE_PCR18 = PORTE_PCR23 & 0xFFFFF8FF;	// Zera bits de MUX de PTB23
	SET_BIT(PORTE_PCR23, 8);				// Seta bit 0 do MUX de PTB23, assim os bits de MUX serao 001
	SET_BIT(GPIOE_PDDR, 23);				// Seta pino 23 do PORTE como saida 
}

void delay(uint32_t t) // Atrasa t microsegundos
{
	__asm volatile(
		"mov r0, %0 \n"
		"mov r1, #2 \n"
		"mul r0,r1 \n"
		"again: \n"
		"cmp r0,#0 \n"
		"beq return \n"
		"sub r0,#1 \n"
		"b   again\n"
		"return: \n"
		".align 2 \n"
		:
		:"r" (t)
		);
	return;
}

