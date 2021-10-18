/*  
	EA871 Q - Roteiro 10
	
	Patrick Carvalho
	
*/


#include "uart.h"
#include "derivative.h" /* include peripheral declarations */
#define tamanho 50


int c, h, m, s; //4 contadores diferentes
int terminal, list, stop, comeco_var, esc, nmi; //variáveis de controle
char buffer[tamanho];
int receptor = 0;
int transmissor = 0;
int detector = 0;
char espaco_livre[10];
c = 0;
m = 0;
h = 0;
s = 0;
stop = 0;
esc=0;
nmi=0;

void cursor_canto_esquerdo(void)
{
	putchar_UART0('\e');
	puts_UART0("[H");
}

void cursor_limpa_tela(void)
{
	putchar_UART0('\e');
	puts_UART0("[2J");		
}

char cast_decimal(int caracter, int potencia) { //função que retorna um dos caracteres decimais referentes a uma letra na tabela ASCII
												//seu parametro é o caracter recebido pelo terminal
	if (potencia == 2) caracter = caracter/100; //o digito a ser retornado depende da variavel potencia
	else if (potencia == 1) caracter = caracter/10 - (caracter/100) * 10;
		
	else caracter = caracter - (caracter/100) * 100 - (caracter/10 - (caracter/100) * 10)*10;
	
	return caracter+48;
}

int cast_char(char inteiro, int potencia) {
	
	inteiro = inteiro - 48;
	
	if (potencia == 2) return inteiro*100;
	else if (potencia == 1) return inteiro*10;
	else if (potencia == 0) return inteiro;
		
	
}

void init_SysTick(){
	
	SYST_CSR |= 0x7; //ativa os enables
	SYST_RVR = 0x33333; //número para que a interrupção aconteça a cada centésimo de segundo
	
}

void SysTick_Handler()
{ //contador através do Systick
	
	c++;
}

void init_NMI()
{
	
	SIM_SCGC5 |= 0x200;
	PORTA_PCR4 &= 0xFFFFF8FF; //zera MUX de PTA4
	PORTA_PCR4 |= 0x00000700; //escolhe alt7 para configuração
}

void NMI_Handler(){
	
	nmi = 1;
}

void init_PORTA_inter(){
	
	SIM_SCGC5 |= 0x200;
	
	PORTA_PCR12 &= 0xFFFFF8FF; //zera MUX de PTA12
	PORTA_PCR12 |= 0x00000100; //escolhe GPIO para configuração
	
	PORTA_PCR12 &= 0xFFF0FFFF; //Habilitar geração pedido de interrupção borda subida
	PORTA_PCR12 |= 0x00090000;
	
	PORTA_PCR12 |= 0x08000000; // Limpar eventual pedido de interrupção que esteja pendente
	
	NVIC_IPR7 &= 0xFF3FFFFF; //Definir prioridade no NVIC 
	
	NVIC_ISER |= 0x40000000;
	
}

void PORTA_IRQHandler(){
	
	if (stop == 0) {
			SYST_CSR &= 0xfd;
			stop = 1;
		}
	
	else {
			SYST_CSR |= 0x02;
			stop = 0;
		}
	
	PORTA_PCR12 |= 0x08000000; // Limpar eventual pedido de interrupção que esteja pendente
}

void init_UART0_inter(){
	
	UART0_C1 &= 0xed; //Configurar TX/RX 8 bits, sem paridade
	UART0_C2 |= 0x2c; //Habilitar transmissor, receptor e interrupção recepção
	
	NVIC_IPR3 &= 0xFFFFFF3F; 
	NVIC_IPR3 |= 0x00000040; //Definir prioridade no NVIC 
	
	NVIC_ISER |= 0x00001000;
	
}

void UART0_IRQHandler(){
	
	// Verifica se a interrupção é de transmissão
		
	if(UART0_S1 & UART0_S1_TDRE_MASK){
		
				if( (receptor-transmissor)== 0) {	// Desabilita transmissão de interrupção caso o buffer esteja vazio
					UART0_C2 &= 0x7f;
				} 
				
				else {										// Condição de buffer não vazio
	
						UART0_D = buffer[transmissor % tamanho];		// Escrita na UART	
						transmissor++;
				}
		}
		
	if ( UART0_S1 & UART0_S1_RDRF_MASK) { // verifica se existe um novo caractere na UART0 (Register Full)
		comeco_var = UART0_D; // lê o caracter
		detector = 1;
	}
}

void contador() {
	
	if ( c >= 100) { //a cada cem centesimos, incrementa-se os segundos
				
				s = s+1;
				c = 0;
			}
			
			if ( s >= 60){ //a cada 60 segundos, passa-se 1 minuto
				
				m = m +1;
				s=0;
			}
			
			if ( m >= 60) { //a cada 60 minutos, passa-se 1 hora
				
				h = h + 1;
				m = 0;
			}
}

void atualizar_string(char *string){
	
		string[10] = cast_decimal(c,0);  //OBS: ao salvar as variáveis na string, não há erro de impressão caso a interrupção se de no meio de um print
		string[9] = cast_decimal(c,1);

			//atualiza os dígitos dos segundos
					
		string[7] = cast_decimal(s,0);
		string[6] = cast_decimal(s,1);
					
			//atualiza os dígitos dos minutos
									
		string[4] = cast_decimal(m,0);
		string[3] = cast_decimal(m,1);
					
			//atualiza os dígitos das horas
													
		string[1] = cast_decimal(h,0);
		string[0] = cast_decimal(h,1);
				
		string[2]= ':';
		string[5] = ':';
		string[8] = ':';
}

void atualizar_espaco_livre(char *espaco) {
	
	int dif;
	
	dif = tamanho - (transmissor % tamanho);
	
	espaco[0] = cast_decimal(dif, 2);
	espaco[1] = cast_decimal(dif, 1);
	espaco[2] = cast_decimal(dif, 0);
}




void putchar_UART0(char c){
	
	if (!stop) {
		
		while(receptor-transmissor == tamanho){ // Caso de Buffer FULL para espera
			
			//do nothing until interupt
			}
			UART0_C2 &= 0x7f;
			buffer[receptor % tamanho] = c;
			receptor++;
		
	}
	
	UART0_C2 |= 0x80;
	
}

int main(void) {
	
	init_SysTick(); //inicia interrupção SysTick
	init_NMI(); //inicia interrupção sem mascara
	init_PORTA_inter(); //inicia interrupção da porta
	init_UART0(); //inicia UART0
	init_UART0_inter(); //inicia interrupção da uart0
	init_GPIO_LCD(); //configura o LCD
	init_LCD(); //inicia o LCD
	clear_LCD();
	
	char cara; //variavel onde se guarda o caracter passado ao terminal
	terminal = 1; //terminal = 0 é o terminal desligado
	list = 0; //list = 0 é a lista desligada
	
	int i, aux, terminal_flag;
	terminal_flag=0;
	
	char string[12]; //a string que conterá o cronômetro
	string[11] = '\0'; //como mudarei os parametros um a um, é necessário definir seu ultimo caracter como o nulo
	

	for ( ; ; ){ //contadores começam em 0, apenas os centésimos são atualizados
		
		contador();
		
		atualizar_string(string);
		atualizar_espaco_livre(espaco_livre);
		
		setpos_LCD(1,0);
		puts_LCD(string); //imprime a string no LCD
		
		setpos_LCD(2,0);
		puts_LCD(espaco_livre);
		
		// Detecta se algum caracter foi apertado, alterando flags
		
		if ( (comeco_var == 'd') && (detector == 1) ) { //caso tenha sido apertado D
												
					if (terminal == 1) //se terminal está ligado,
						terminal = 0; //desliga o terminal
						
					else terminal = 1; //caso contrário, liga o terminal
					
					detector = 0;
						
		}
			
		else if ( (comeco_var == 'l') && (detector == 1)) { //caso seja apertado L
														
				if (list == 0) list = 1; //ativa modo lista se desligado

				else list = 0; //desativa o modo lista se ligado
				
				detector = 0;
							
		}
			
		else if ( (comeco_var == 27) && (detector == 1) ) {
			
			esc = 1;
			detector = 0;
		}
		
		// Age de acordo com as flags alteradas
			
		if (list == 1) puts_UART0("\n"); //se a lista foi ativada, pula uma linha antes de imprimir
		
		if (terminal == 1) {
			
			puts_UART0("\r"); //retorna o cursor para o início da linha
			puts_UART0(string); //se a tecla D foi ativada, imprimi no terminal
			terminal_flag = 0;
	
		}
			
		if ( (terminal == 0) && !terminal_flag){
				
				cursor_limpa_tela();
				cursor_canto_esquerdo(); //seta cursor na posição inicial e limpa a tela
				cursor_limpa_tela();
				terminal_flag = 1;
				
			}
	
		
		if (esc == 1) {
			
			cursor_limpa_tela();
			cursor_canto_esquerdo(); //limpa tela do terminal e coloca cursor na posição original
			cursor_limpa_tela();
			clear_LCD(); //clear LCD
			return 0; //acaba o programa
			
		}
		
		if (nmi == 1)
		{
			
			c = 0;
			m = 0;
			h = 0;
			s = 0;
				
			nmi = 0;		
						
		}
				
	}
				
	nmi = 0;
				
	SYST_CSR |= 0x02;
}
		


