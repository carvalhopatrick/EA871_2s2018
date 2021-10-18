// Roteiro 2 - Programa 2b - EA871-2S2017 - Le botoes de pressao da placa Shield

#include "derivative.h" /* include peripheral declarations */
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))	// Macro que seta o bit idx do registrador reg

/* Inicializa os GPIOs */
void init_GPIO(void)
{
	// LEDs
	SET_BIT(SIM_SCGC5, 11);		// Habilita clock do PORTC (System Clock Gating Control)
		
	PORTC_PCR0 &= 0xFFFFF8FF;	// Configura MUX para PTC0-PTC7 e PTC10 aparecerem nos pinos
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
	PORTC_PCR10 &= 0xFFFFF8FF;
	SET_BIT(PORTC_PCR10, 8);
	
	GPIOC_PDOR = 0;				// Garante nivel 0 em todas as saidas do PORTC
	GPIOC_PDDR = 0x000004FF;	// Configura bits 0-7 e 10 do PORTC como saidas
	GPIOC_PSOR = (1<<10);       // Habilita o LE (Latch Enable) do registrador dos LEDs vermelhos (74HC573)
	GPIOC_PCOR = (1<<10);		// Desabilita o LE (Latch Enable) do registrador dos LEDs vermelhos (74HC573)
	
	// Botoes
	SET_BIT(SIM_SCGC5, 9); 		// Habilita clock do PORTA
	
	PORTA_PCR4 &= 0xFFFFF8FF;	// Configura MUX para PTA4, PTA5, PTA12 aparecerem nos pinos
	SET_BIT(PORTA_PCR4, 8);
	PORTA_PCR5 &= 0xFFFFF8FF;
	SET_BIT(PORTA_PCR5, 8);
	PORTA_PCR12 &= 0xFFFFF8FF;
	SET_BIT(PORTA_PCR12, 8);

	GPIOA_PDDR = 0; 		// Configura todos bits do PORTA como entrada
	
}

int main(void)
{
	
	init_GPIO();
	
	uint16_t a, pta4, pta5, pta12;
	uint16_t pta4_old, pta5_old, pta12_old;
	
	pta4  = 0;
	pta5  = 0;
	pta12 = 0;
	
	for( ; ; )
	{
		a = GPIOA_PDIR;       // Faz a leitura no PORTA (16 bits)

		pta4_old = pta4;
		pta5_old = pta5;
		pta12_old = pta12;
		
		pta4  = a & (1<<4);
		pta5  = a & (1<<5);  
		pta12 = a & (1<<12);
		
		pta5  = pta5  >> 5;
		pta4  = pta4  >> 4;
		pta12 = pta12 >> 12;

		if (pta4_old == 0 && pta4 == 1)		// Se o botao foi solto
		{
			SET_BIT(GPIOC_PTOR, 3);				// Altera o estado do LED D3
		}


		
	}
}