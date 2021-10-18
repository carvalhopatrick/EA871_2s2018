/*
 * Roteiro 04
 * EA871 S  -  2s2018
 *
 * Patrick Penacho Carvalho
 * RA 185388
 *
 * O programa soma dois valores da memoria RAM (representados no formato Float IEEE 754)
 * Em seguida, armazena o resultado também na memória RAM.
 *
 * Formula utilizada:
 *		X + Y = (Xs * 2^(Xe - Ye) + Ys) * 2^Ye 
 * Sendo:
 *      X <= Y (menor ou igual)
 *		Xs,Ys = Mantissa de X/Y, com o bit oculto
 *		Xe,Ye = Expoente de X/Y
 *
 */
 
	
/* Area de dados em memoria RAM - escrita e leitura */
	.data			
	.align	2
	
oper1:					/* operando 1 */
	.word 	0b01000001101100111010010111100011
	
oper2:					/* operando 2 */
	.word 	0b11000001000000000001010101001101

	
result:					/* resultado */
	.word 0x00000000
	

/* Area de dados em memoria de flash (apenas leitura) */
	.text			
	.align	2
	.global	main
	.type main function

/* Roda caso oper1 = 0. Resultado = oper2 */
zero1:
	mov r4,r2	/* Carrega R2 (oper2) em R4 (resultado) */
	b final		/* Pula para final do programa /*

/* Roda caso oper2 = 0. Resulado = oper1 */	
zero2:
	mov r4,r1	/* Carrega R1 (oper1) em R4 (resultado) */
	b final		/* Pula para final do programa /*
	
	
/* Se mantissa + exp de oper1 e 2 forem iguais, roda esse trecho para checar se sinais sao opostos */
testesinal:
	mov r4,r1		/* Carrega R1 (oper1) em R4 */
	lsr r4,r4,#31	/* Isola o bit de sinal */
	mov r5,r2		/* Carrega R2 (oper2) em R5 */
	lsr r5,r5,#31	/* Isola o bit de sinal */
	cmp r4,r5		/* Compara os dois sinais */
	beq naoopostos  /* Se nao forem opostos, volta para rotina principal */
	
	mov r4,#0		/* Carrega valor zero em R4 (resultado) */
	b final 		/* Pula para final do programa */
	
	
/* Rotina de subtracao. Roda se sinais forem opostos, mas modulos diferentes */
subtracaoMAIN:
/* No momento:
	R1  = sinal de oper1
	R2  = sinal de oper2
	R6  = 1 se R1>R2 (oper1 = Y),  0 se R1<R2 (oper1 = X)     (inversao)
	R9  = Xs						
	R10 = Ys
	R11 = R12 = Xe = Ye				*/
	
	/* Inverte os sinais armazenados em R1 e R2, caso foram invertidos os operandos anteriormente */
	cmp r6,#1
	bne naoinvSINAL
	mov r3,r1
	mov r1,r2
	mov r2,r3

naoinvSINAL:
	/* Neste ponto, R1 possui o sinai relacionado a X, e R2 o sinal relacionado a Y. /*
	
	/* Determina a ordem da subtracao. Pula para rotina da ordem correta */
	cmp r1,#1
	bne subtracaoXY		/* Se X nao for negativo, Y sera. pula para rotina de subt. X-Y */ 
	
	/* Se sinal de X = 1 (negativo), faz subtracao Y-X */
	mov r1,r10		/* Carrega Ys em R1 */
	mov r2,r9		/* Carrega Xs em R2 */
	sub r6,r1,r2    /* Faz Ys-Xs. Armazena resultado em R6 */
	cmp r1,r2		/* Compara R1 com R2 */
	bpl	subPOSITIVO	/* Se R1>R2, sinal final sera positivo */
	b subNEGATIVO
	
subtracaoXY:	
	/* Se sinal de X = 0 (positivo), faz subtracao X-Y */
	mov r1,r9		/* Carrega Xs em R1 */
	mov r2,r10		/* Carrega Xs em R2 */
	sub r6,r1,r2   /* Faz Xs-Ys. Armazena resultado em R6 */
	cmp r1,r2		/* Compara R1 com R2 */
	bpl	subPOSITIVO	/* Se R1>R2, sinal final sera positivo */
	b subNEGATIVO
	
subPOSITIVO:
	mov r1,#0
	mov r8,r1		/* R8 guardara o bit de sinal = 0 */
	b normMAIN		/* Pula para secao de normalizacao da rotina principal */
	
subNEGATIVO:
	mov r1,#1
	mov r8,r1			/* R8 guardara o bit de sinal = 1 */
	ldr r1,NEGATIVO1	/* Carrega um valor -1 em complemento de 2 em R1 */
	mul r6,r6,r1		/* Multiplica o resultado (R6) por -1 para complementa-lo */	
	b normMAIN			/* Pula para secao de normalizacao da rotina principal */
	
	
/* Programa principal */
main:
	/* inicialização main */
	push {r3, lr}
	add	r3, sp, #4
	
	/* Carrega oper1 em R1 e oper2 em R2 */
	ldr r0, oper1_ptr
	ldr r1,[r0]
	ldr r0, oper2_ptr
	ldr r2, [r0]
	
	/* Checa se um dos operandos equivale a 0. */ 
	mov r4,r1
	lsl r4,r4,#1  /* Elimina o bit de sinal, para comparacao */
	cmp r4,#0
	beq zero1     /* Caso R1 (oper1) seja 0, pula para zero1 */
	
	mov r4,r2
	lsl r4,r4,#1 
	cmp r4,#0
	beq zero2
	
	/* Checa se os operandos sao inversos um do outro. */
	mov r4,r1		/* Carrega R1 (oper1) em R4 */
	lsl r4,r4,#1	/* Elimina o bit de sinal */
	mov r5,r2		/* Carrega R2 (oper2) em R5 */
	lsl r5,r5,#1	/* Elimina o bit de sinal */
	cmp r4,r5		/* Compara os dois operandos (mantissa + exp) */
	beq testesinal	/* Se forem iguais, pula para rotina "testesinal" */
		
		
naoopostos:		
/* Define os expoentes Xe e Ye, sendo Xe <= Ye	*/	
	/* Guarda expoentes de oper1 em R1 e oper2 em R2, deslocando para eliminar bits da mantissa */
	ldr r0,EXPMASK
	and r1,r1,r0
	lsr r1,r1,#23
	
	and r2,r2,r0
	lsr r2,r2,#23
	
	/* Se R1>R2, inverte R1 com R2. Guarda 1 em R6 caso necessite a inversao */
	mov r6,#0
	cmp r2,r1		/* Compara R2 com R1 (R2-R1) */
	bpl naoinvEXP  	/* Se R2-R1 = positivo, R1<R2, nao havera inversao */ 
	mov r6,#1
	
	mov r3,r1
	mov r1,r2
	mov r2,r3
	
	
naoinvEXP:
/* Ajusta Xe de tal forma que Xe = Ye. Guarda Xe em R11 e Ye em R12 */
	/* Guarda a diferenca dos expoentes em R7 */	
	sub r7,r2,r1
	
	/* Se Xe = Ye (R7 = 0), nao e necessario executar esta parte */
	/*cmp r7,#0
	beq naoajusta*/
	
	/* Soma a diferenca dos exp. em Xe (menor expoente) */
	add r1,r1,r7

	/* Guarda R1 em R11 (Xe) e R2 em R12 (Ye) */
	mov r11,r1
	mov r12,r2


/* Define Xs (R9) e Ys (R10) (mantissa + o bit oculto). */
	/* Carrega oper1 em R1 e oper2 em R2 */
	ldr r0, oper1_ptr
	ldr r1,[r0]
	ldr r0, oper2_ptr
	ldr r2, [r0]
	
	/* Guarda mantissa de oper1 em R1 e oper2 em R2 */
	ldr r0,MANTMASK
	and r1,r1,r0
	and r2,r2,r0
	
	/* Inverte R1 e R2, caso o expoente de oper1 seja > que o de oper2 (R6 = 1) */
	cmp r6,#1
	bne naoinvMANT
	
	mov r3,r1
	mov r1,r2
	mov r2,r3


naoinvMANT:	
	/* Adiciona o bit oculto a mantissa armazenada em R1 e R2 */
	ldr r0,OCULTO
	orr r1,r1,r0
	orr r2,r2,r0
	
	/* Desloca Xs de acordo com o ajuste feito para igualar Xe com Ye */
	lsr r1,r1,r7  /* R7 guarda a diferença original entre os expoentes */

	/* Guarda R1 em R9 (Xs) e R2 em R10 (Ys) */	
	mov r9,r1
	mov r10,r2	
	
/* Testa se devera ocorrer soma ou subtracao (dependendo dos sinais) */
	/* Carrega oper1 em R1 e oper2 em R2 */
	ldr r0, oper1_ptr
	ldr r1,[r0]
	ldr r0, oper2_ptr
	ldr r2, [r0]
	
	lsr r1,r1,#31		/* Isola os bits de sinal */
	lsr r2,r2,#31
	cmp r1,r2			/* Compara R1 com R2 */
	bne subtracaoMAIN	/* Se os sinais forem diferentes, pula p/ rotina de subtracao /*
	
	
	/* Se sinais forem iguais, guarda sinal do resultado em R8 */
	mov r8,r1

/* Soma as mantissas Xs e Ys. Guarda a soma em R6 */
naoajusta:
	mov r6,r9
	add r6,r6,r10

normMAIN:
/* Normaliza a mantissa resultante da soma, ajustando o expoente de acordo */
	/* Checa se houve carry. */
	mov r5,r6
	lsr r5,r5,#24
	cmp r5,#1
	bne normSHIFT 	/* Se nao houve carry, pula para normalizacao por shift */
	
	/* Se houve carry, desloca mantissa 1 bit para direita e soma 1 ao expoente Zs */
	lsr r4,r6,#1
	mov r3,r11
	add r3,r3,#1
	b normalizado
	
	
normSHIFT:
	/* 	Subtrai 1 de R1 ate achar o numero de bits deslocados a esquerda necessarios 
		para normalizar a mantissa resultante. Esse numero sera armazenado em R1 	 */
	mov r1,#24
	mov r0,#1
normloop:
	sub r1,r1,#1  /* Subtrai 1 de R1 */
	mov r5,r6
	lsr r5,r5,r1  /* Desloca R6 [R1] bits a direita e guarda em R5 */
	and r5,r5,r0  /* R5 = 0 ou 1 (zera bits 1-31) */
	cmp r5,#0
	beq normloop  /* Se R5 = 0, roda o loop novamente */
	
	/* Acha o numero necessario para shift left e armazena em R5 */
	mov r5,#23
	sub r5,r5,r1
	
	/* Normaliza a mantissa, deslocando [R5] bits para a esquerda. Armazena em R4 */
	mov r4,r6
	lsl r4,r4,r5
		
	/* Ajusta o expoente, subtraindo [R5] do expoente. Armazena em R3. */
	mov r3,r11
	sub r3,r3,r5

/* Montagem do resultado em IEEE 754 
		Agora: R3 = Ze  ;  R4 = Zs 	;  ***R8 = Zsinal*s**  */
normalizado:
	/* Elimina os bits 23-31 da mantissa armazenada (oculta bit oculto) */
	ldr r0,MANTMASK
	and r4,r4,r0
	
	/* Desloca o expoente 23 bits para a esquerda */
	lsl r3,r3,#23
	
	/* Junta a mantissa, expoente e sinal. Armazena em R4. */
	orr r4,r4,r3
	mov r3,r8
	orr r4,r4,r3
	
	/* armazena resultado final (oper1 + oper2) na memória (result) */
final:
	ldr r0, result_ptr
	str r4,[r0]
	
	
	
	
	
	
	/* retorno main */
	mov	r3, #0
	mov	r0, r3
	pop {r3, pc}
	

/* Area de dados em memoria Flash - somente leitura */
	.align	2
	
/* apontador para operando 1 */
oper1_ptr:
	.word oper1
	
/* apontador para operando 2 */
oper2_ptr:
	.word oper2
	
/* apontador para resultado */
result_ptr:
	.word result

/* Mascara para isolar os bits de expoente de um numero em IEEE 754 */
EXPMASK:
	.word	0b01111111100000000000000000000000

/* Mascara para isolar os bits de mantissa de um numero em IEEE 754 */
MANTMASK:
	.word 	0b00000000011111111111111111111111
	
/* Mascara para adicionar o bit oculto a mantissa */	
OCULTO:
	.word	0b00000000100000000000000000000000

/* Valor -1 em complemento de 2 */
NEGATIVO1:
	.word	0xFFFFFFFF
	
	
/* Fim area memoria Flash */
	.end

	
