/*
 * Roteiro 09
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




void delay(unsigned long t) // Atrasa t microsegundos
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

