/*
 * Roteiro 09
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * 			init.c
 *
 */
 
#include "derivative.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))
 
/* Configura interrupcoes para entradas S1 e S3 */
void init_GPIO()
{
	SIM_SCGC5 |= 0x200;		// Habilita clock do PORTA (System Clock Gating Control)
	PORTA_PCR4 |= 0x700; 	// Seta o MUX de PTA4 (S1) para 111 (Alt 7 - NMIb)	
	
	/* ===== PTA12 - Botao S3 ==== */
	// Configura MUX de PTA12 (Alt 1)
	PORTA_PCR12 &= 0xFFFFF8FF;		
	PORTA_PCR12 |= 0x00000100;
	
	// IRQC <-- 1001 (Interrupt on rising edge)
	PORTA_PCR12 &= 0xFFF0FFFF; 		
	PORTA_PCR12 |= 0x00090000;
	
	PORTA_PCR12 |= 0x08000000;	// Seta Interrupt Status Flag
	
	
	// Define maior prioridade para interrupção 30 (Pin Detect A)
	NVIC_IPR7 &= 0xFF3FFFFF;	// Enable IRQ30
	
	// Habilita interrupção 30 (Pin Detect A)
	NVIC_ISER |= 0x40000000;
}
 
/* Configura SysTick para gerar 1 interrupcao por centésimo */
void init_SysTick()
{	
	SYST_CSR = 0x00000007;	// CLKSOURCE = 1; TICKINT = 1; ENABLE = 1
	SYST_RVR = 209715;		// CLK do ARM = 20,97152 MHz. 209715 gera 1 interrup. por centesimo
}
 
