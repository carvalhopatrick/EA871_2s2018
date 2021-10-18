/*
 * 		Roteiro 01 
 *   EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 */

#define SIM_SCGC5   (*(unsigned int volatile *) 0x40048038)	// System Clock Gating Control Register 5 (clocks do GPIO) (Reg. SIM_SCGC5)

#define PORTB_PCR18 (*(unsigned int volatile *) 0x4004A048)	// Pin Control Register (MUX) de PTB18 (Reg. PORTB_PCR18)
#define PORTB_PCR19 (*(unsigned int volatile *) 0x4004A04C)	// Pin Control Register (MUX) de PTB19 (Reg. PORTB_PCR18)
#define PORTD_PCR1  (*(unsigned int volatile *) 0x4004C004)	// Pin Control Register (MUX) de PTD1  (Reg. PORTD_PCR1)

#define GPIOB_PDDR  (*(unsigned int volatile *) 0x400FF054)	// Data direction register do PORTB (Reg. GPIOB_PDDR)
#define GPIOB_PTOR  (*(unsigned int volatile *) 0x400FF04C)	// Port Toggle Output register do PORTB (Reg. GPIOB_PTOR)
#define GPIOB_PSOR  (*(unsigned int volatile *) 0x400FF044)	// Port Set Output register do PORTB (Reg. GPIOB_PSOR)

#define GPIOD_PDDR  (*(unsigned int volatile *) 0x400FF0D4)	// Data direction register do PORTD (Reg. GPIOD_PDDR)
#define GPIOD_PTOR  (*(unsigned int volatile *) 0x400FF0CC)	// Port Toggle Output register do PORTD (Reg. GPIOD_PTOR)
#define GPIOD_PSOR  (*(unsigned int volatile *) 0x400FF0C4)	// Port Set Output register do PORTD (Reg. GPIOD_PSOR)


#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))	// Macro que seta o bit idx do registrador reg

#define TOGGLE_R	SET_BIT(GPIOB_PTOR, 18);		// Macro que alterna o estado do pino conectado ao LED vermelho
#define TOGGLE_G	SET_BIT(GPIOB_PTOR, 19);		// Macro que alterna o estado do pino conectado ao LED verde
#define TOGGLE_B	SET_BIT(GPIOD_PTOR, 1);			// Macro que alterna o estado do pino conectado ao LED azul

#define TEMPO	1500000		// Tempo de delay entre cada acionamento da sequencia de LEDs

void delay(unsigned int time)  
{
	for ( ; time != 0; time--) ;  		// espera antes de retornar
}

int main(void)
{
	SET_BIT(SIM_SCGC5, 10);		   	// Habilita clock do PORTB (System Clock Gating Control)
	SET_BIT(SIM_SCGC5, 12);		   	// Habilita clock do PORTD (System Clock Gating Control)
	
	PORTB_PCR18 = PORTB_PCR18 & 0xFFFFF8FF;		// Zera bits de MUX de PTB18
	PORTB_PCR19 = PORTB_PCR19 & 0xFFFFF8FF;		// Zera bits de MUX de PTB19
	PORTD_PCR1  = PORTD_PCR1  & 0xFFFFF8FF;		// Zera bits de MUX de PTD1
	
	SET_BIT(PORTB_PCR18, 8);		// Seta bit 0 do MUX de PTB18, assim os bits de MUX serao 001 (habilita "Alt 1" da tebela de pinout)
	SET_BIT(PORTB_PCR19, 8);		// Seta bit 0 do MUX de PTB19, assim os bits de MUX serao 001
	SET_BIT(PORTD_PCR1,  8);		// Seta bit 0 do MUX de PTB19, assim os bits de MUX serao 001
	
	SET_BIT(GPIOB_PDDR, 18);		// Seta pino 18 do PORTB como saida 
	SET_BIT(GPIOB_PDDR, 19);		// Seta pino 19 do PORTB como saida 
	SET_BIT(GPIOD_PDDR, 1);			// Seta pino 19 do PORTB como saida 
	
	SET_BIT(GPIOB_PSOR, 18);		// Seta bit 18 em PTB (Desliga LED Vermelho)
	SET_BIT(GPIOB_PSOR, 19);		// Seta bit 19 em PTB (Desliga LED Verde)
	SET_BIT(GPIOD_PSOR, 1);			// Seta bit 1  em PTD (Desliga LED Azul)
	
	for( ; ; )  	// loop infinito
	{	   
		/* Sequencia dos LEDs:
		 *		R-r-G-g-R-r-B-b-R-r-GB-gb-R-r-RB-rb-R-r-RG-rg--
		 *		R--r--G--g--R--r--B--b--R--r--GB--gb--R--r--RB--rb--R--r--RG--rg-
		 */
		
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_G;		
		delay(TEMPO);
		TOGGLE_G;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_B;		
		delay(TEMPO);
		TOGGLE_B;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		
		TOGGLE_G;
		TOGGLE_B;
		delay(TEMPO);
		TOGGLE_G;
		TOGGLE_B;
		delay(TEMPO);
		
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_B;
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_B;
		delay(TEMPO);
		
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_G;
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_G;
		delay(TEMPO);
		delay(TEMPO);		
		
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_G;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_G;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_B;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_B;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		
		TOGGLE_G;
		TOGGLE_B;
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_G;
		TOGGLE_B;
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_B;
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_B;
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;		
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_G;
		delay(TEMPO);
		delay(TEMPO);
		TOGGLE_R;
		TOGGLE_G;
		delay(TEMPO);
	}
}