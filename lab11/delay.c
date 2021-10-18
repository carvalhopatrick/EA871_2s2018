/*  
	EA871 Q - Roteiro 11
	
	Patrick Carvalho
	
*/

#include "delay.h"
#include "derivative.h" /* include peripheral declarations */

void delay(uint32_t t){		// deve atrasar t microsegundo
	
		__asm volatile(
				"mov r0, %0 \n"
				"mov r1, #2 \n"
				"ldr r2,multi \n"
				"mul r0,r1 \n"
				"mul r0,r2 \n"
				"again: \n"
				"cmp r0,#0 \n"
				"beq return \n"
				"sub r0,#1 \n"
				"b   again\n"
				"return: \n"
				".align 2 \n"
				"multi: \n"
				".word 2 \n" //garante que 't' é dado em microsegundos
				:
				:"r" (t)
				);
		return;
}
