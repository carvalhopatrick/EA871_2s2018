/*  
	EA871 Q - Roteiro 11
	
	Patrick Carvalho
	
*/

#include "derivative.h"
#include "lcd.h"
#include "delay.h"

#define SET_BIT(reg, idx)  reg = (reg | (1 << idx)) 

void init_GPIO(void);
void putsDuty_display(int);
void NMI_Handler();
void PORTA_IRQHandler();

int dutyTPM = 0;
int dutyDisplay = 0;
int dutyDisplay_old = 0;
char str[5];

int main()
{
	init_GPIO();
	init_GPIO_LCD();
	init_LCD();
	
	puts_LCD("000%");
	
	// Loop infinito (desvia para interrupçoes e atualiza display)
	for( ; ; )	
	{
		if (dutyDisplay != dutyDisplay_old)	// se duty cycle foi alterado por interrupçoes
		{
			putsDuty_display(dutyDisplay);				// coloca novo valor no display
			dutyDisplay_old = dutyDisplay;
		}
	} 
}

// Rotina de interrupcao do botao S1
void NMI_Handler() //s1 - zera duty
{
	TPM1_C0V = 0x00;
	dutyTPM = 0;
	dutyDisplay = 0;
}

// Rotina de interrupcao dos botoes ligados em PORT A (S2 e S3)
void PORTA_IRQHandler()
{
	// checa se interrupcao foi causada pelo pressionamento de S3
	if (PORTA_PCR12 & (1<<24)) //s3 - aumenta 5% duty
	{
		if (dutyDisplay <= 95)
		{
			dutyTPM += 1310;
			TPM1_C0V = dutyTPM;			// seta novo duty cycle

			dutyDisplay += 5;
		}
		
	}
	
	// checa se interrupcao foi causada pelo pressionamento de S2
	if (PORTA_PCR5 & (1<<24)) //s2 - diminui 5% duty
	{
		if (dutyDisplay >= 5)
		{
			dutyTPM -= 1310;
			TPM1_C0V = dutyTPM;			// seta novo duty cycle

			dutyDisplay -= 5;	
		}
		
	}
	
	// limpa flags de interrupcao
	SET_BIT(PORTA_PCR12, 24);
	SET_BIT(PORTA_PCR5, 24);
}

// Inicializacao de portas e modulos
void init_GPIO(void)
{
	
	SET_BIT(SIM_SCGC5,9);    // Habilita clock PORTA
	SET_BIT(SIM_SCGC5,10);   // Habilita clock portaB	
	
	PORTA_PCR4 &= 0xFFFFF8FF; // zera MUX de PTA4
	PORTA_PCR4 |= 0x00000700; // Alt7 para pin4 (NMI)
	
	
	PORTA_PCR12 &= 0xFEFFF8FF; // zera MUX de PTA12
	PORTA_PCR12 |= 0x00000100; // Alt1 para pin14 -> PTA12, borda de subida, clear interrupção request
	PORTA_PCR12 |= 0x00090000;
	
	PORTA_PCR5 &= 0xFEFFF8FF; // zera MUX de PTA14
	PORTA_PCR5 |= 0x00000100; // Alt1 para pin14 -> PTA12, borda de subida, clear interrupção request
	PORTA_PCR5 |= 0x00090000;
	
	PORTB_PCR0 &= 0xFFFFF8FF; // zero MUX de PTB0
	PORTB_PCR0 |= 0x00000B00; // ALT 3
	
	// interrupcoes botoes
	SET_BIT(NVIC_IPR7, 23);    // set interrupt priority associate with portA at reg. NVIC_IPR7
	SET_BIT(NVIC_ISER, 30);    // enable the interrupt associate with portA 
	
	// =========  PWM  ============
	SET_BIT(SIM_SCGC6,25);   // Habilita clock TPM
	
	// Seleciona clock MCGFLLCLK para TPM (20,97 MHz)
	SIM_SOPT2 = SIM_SOPT2 & 0xF3FFFFFF; 
	SET_BIT(SIM_SOPT2, 24);  
	
	// configura SC (status and control):
		// prescaler = divide by 4					5,243 MHz
		// habilita contgem para cada clock LPTPM
		// habilita modo PWM simetrico contagem up-down
	TPM1_SC  = 0;			
	TPM1_SC  = 0x0000002A;
	
	// modulo = 26214
	TPM1_MOD = 0x6666;
	
	TPM1_C0V = 0X0;
	TPM1_C0SC |= 0x00000068; // configs extras
}

void putsDuty_display(int duty)
{
	int c=0, d=0, u=0;	// centena, dezena e unidade do duty cycle
	
	c = duty / 100;
	d = (duty / 10) - (c * 10);
	u = (duty - (d * 10)) - (c * 100);
	
	clear_LCD();
	str[0] = c+48;
	str[1] = d+48;
	str[2] = u+48;
	str[3] = '%';
	str[4] = '\0';
	puts_LCD(str);
	
}
