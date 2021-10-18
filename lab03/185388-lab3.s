/*
 * Roteiro 03 
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * O programa executa a seguinte sequencia no LED RGB da placa Freedom:
 * RGB - 0 - G - R - RG - B - BG - RB - RGB - 0 - G - R -
 * A sequencia é repetida infinitamente
 *
 */

/* This assembly file uses GNU syntax */
	.text
    	.align 	   2
    	.global    main
    	.type main function


@-----------------------------------
@// Funcao toggle - inverte o estado de um dos leds
@// Entradas:
@//  r1 - com #64 para o LED vermelho, #128 para o LED verde, #1 para o LED azul
@// Altera:
@//  - r2, r3
@//  - flags
@-----------------------------------
toggle:
	push {lr}
	cmp r1,#2
	beq blue

	ldr r3,GPIOB_PTOR 		@ // Executa ------ para ----- ou vermelho
	lsl r1,r1,#12			@ // ------- apenas ---- verde -----------
	
condt:
	ldr r2,[r3]
	orr r2,r1
	str r2,[r3]
	b return

blue:
	ldr r3,GPIOD_PTOR     @ // Executa ------ para ----
	b condt				  @ // ------- apenas ---- azul
	
return:
	pop {pc}


@-----------------------------------
@// Funcao de gastar tempo
@// Entradas:
@//  r0 - com a valor de tempo
@// Altera:
@//  - flags
@-----------------------------------
delay:
 	push {lr}
	ldr r0,WAIT
again:
 	cmp r0,#0
 	beq return
	sub r0,#1
	b   again
	
	
@-----------------------------------
@// Funcao Main
@-----------------------------------
main:
@// Setup das portas:

@//Habilita clock do PORTB (10) e PORTD (12) (System Clock Gating Control)
	ldr r3,SIM_SCGC5
	ldr r2,[r3]
 	movs r1,#5			@// 101
  	lsl r1,r1,#10		@// 1010000000000 (bits 10 e 12)
	orr r2,r1
  	str r2,[r3]
  	
@//Zera bits de MUX de PTB18
 	ldr r3,PORTB_PCR18
  	ldr r2,[r3]
   	ldr r1,MUXMASK
  	and r2,r1
   	str r2,[r3]
   	
@//Seta bit 0 do MUX de PTB18, assim os bits de MUX serao 001
  	ldr r3,PORTB_PCR18
  	ldr r2,[r3]
    movs r1,#128		@// 10000000
	lsl r1,r1,#1		@// 100000000 (bit 8)
  	orr r2,r1
   	str r2,[r3]
	
@//Zera bits de MUX de PTB19
 	ldr r3,PORTB_PCR19
  	ldr r2,[r3]
   	ldr r1,MUXMASK
  	and r2,r1
   	str r2,[r3]
   	
@//Seta bit 0 do MUX de PTB19, assim os bits de MUX serao 001
  	ldr r3,PORTB_PCR19
  	ldr r2,[r3]
    movs r1,#128		@// 10000000
	lsl r1,r1,#1		@// 100000000 (bit 8)
  	orr r2,r1
   	str r2,[r3]
   	
@//Zera bits de MUX de PTD1
 	ldr r3,PORTD_PCR1
  	ldr r2,[r3]
   	ldr r1,MUXMASK
  	and r2,r1
   	str r2,[r3]
   	
@//Seta bit 0 do MUX de PTD1, assim os bits de MUX serao 001
  	ldr r3,PORTD_PCR1
  	ldr r2,[r3]
    movs r1,#128		@// 10000000
	lsl r1,r1,#1		@// 100000000 (bit 8)
  	orr r2,r1
   	str r2,[r3]
   	
   	
@//Seta pino 18 e 19 do PORTB como saida 
  	ldr r3,GPIOB_PDDR
   	ldr r2,[r3]			
    movs r1,#3			@// 11
	lsl r1,r1,#18		@// 11000000000000000000 (bits 18 e 19)
  	orr r2,r1
 	str r2,[r3]
	
	
@//Seta pino 1 do PORTD como saida 
  	ldr r3,GPIOD_PDDR
   	ldr r2,[r3]
    movs r1,#2			@// 10
  	orr r2,r1
 	str r2,[r3]


loop:		@// Codigo principal - cria a sequencia de LEDs repetidamente (RGB - 0 - G - R - RG - B - BG - RB - RGB - 0 - G - R)

@//Liga os 3 LEDs (estado inicial), setando-os em PORTx_PCOR
	ldr r3,GPIOB_PCOR
	ldr r2,[r3]
	movs r1,#3
	lsl r1,r1,#18		@// 11000000000000000000 (bits 18 e 19 - verm e verde)
	orr r2,r1
	str r2,[r3]
	
	ldr r3,GPIOD_PCOR
	ldr r2,[r3]
	movs r1,#2			@// 10 (bit 1 - azul)
	orr r2,r1
	str r2,[r3]
	
@//Espera um tempo
  	bl delay 
	@// RGB -

	ldr r1,RED
	bl toggle
	ldr r1,GREEN
	bl toggle
	ldr r1,BLUE
	bl toggle
	bl delay
	@// RGB - 0
	
	ldr r1,GREEN
	bl toggle
	bl delay
	@// RGB - 0 - G
	
	ldr r1,GREEN
	bl toggle
	ldr r1,RED
	bl toggle
	bl delay
	@// RGB - 0 - G - R
	
	ldr r1,GREEN
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG
	
	ldr r1,GREEN
	bl toggle
	ldr r1,RED
	bl toggle
	ldr r1,BLUE
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B
	
	ldr r1,GREEN
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B - BG
	
	ldr r1,GREEN
	bl toggle
	ldr r1,RED
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B - BG - RB
	
	ldr r1,GREEN
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B - BG - RB - RGB
	
	ldr r1,GREEN
	bl toggle
	ldr r1,RED
	bl toggle
	ldr r1,BLUE
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B - BG - RB - RGB - 0
	
	ldr r1,GREEN
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B - BG - RB - RGB - 0 - G
	
	ldr r1,GREEN
	bl toggle
	ldr r1,RED
	bl toggle
	bl delay
	@// RGB - 0 - G - R - RG - B - BG - RB - RGB - 0 - G - R
	
	b loop
	
	
    .align	2

@// Endereços:
SIM_SCGC5:
	.word	0x40048038
PORTB_PCR18:
	.word	0x4004A048
PORTB_PCR19:
	.word	0x4004A04C
PORTD_PCR1:	
	.word	0x4004C004

GPIOB_PDDR:
	.word	0x400FF054 
GPIOB_PTOR:
	.word	0x400FF04C
GPIOB_PCOR:
	.word 	0x400FF048
	
GPIOD_PDDR:
	.word	0x400FF0D4
GPIOD_PTOR:
	.word	0x400FF0CC
GPIOD_PCOR:
	.word	0x400FF0C8

@// Constantes:
MUXMASK:
	.word	0xFFFFF8FF
WAIT:
	.word	5000000
RED:
	.word	64
GREEN:
	.word	128
BLUE:
	.word 	2

@// Final do arquivo assembly
.end