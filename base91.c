/*
 *  Universidade de Brasília
 *  Instituto de Ciencias Exatas
 *  Departamento de Ciência da Computação
 *
 *  Software Básico - Turma A - 1/2018
 *
 *  Base 91
 *
 *  Grupo:
 *      - Felipe Augusto R. Brandão
 *      - Laís Mendes Gonçalves
 *		- Caio Oliveira
 *		- Adriano Torres
 *		- Nícolas Machado Schumacher
 *
 *
 *  Copyright © 2018 UnB. All rights reserved.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include"base91.h"

const char b91[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz"
					"0123456789"
					"!#$>?@[]^_`{|}~\"%&()*+,./:;<=" ;

int converter_para_txt(char* arquivo_entrada, char* arquivo_saida){
	FILE *arquivo, *resultado;
	uint8_t b[13];
	char c[16];
	int vetor[8];

	arquivo = fopen(arquivo_entrada, "r");
	resultado = fopen(arquivo_saida, "w");
	// arquivo = fopen("saida.txt", "r");
	// resultado = fopen("decodificado.txt", "wb");
	char leitura[16];
	int sair;

	while((sair = fread(c, sizeof(uint8_t), 16 , arquivo)) > 0){
		int tamanho_vetor, n;
		int tamanho_for = 13;
	
		tamanho_vetor = encontrar_x(c, sair, vetor);
		
		

		if (sair != 16)
		{
			tamanho_for = (tamanho_vetor*13)/8;
			
		}
		else
		{	
			n = (tamanho_vetor * 13)%8;
			if ((vetor[7] == (8191-n)) || (vetor[7] == 8183))
			{
				tamanho_for = tamanho_for-2;
			}
			else if(b[12]==13)
			{
				tamanho_for = 12;
			}
		}
		

		b[0] = (uint8_t)(vetor[0] >> 5);
		b[1] = (uint8_t)((vetor[0] << 3) | (vetor[1] >> 10));
		b[2] = (uint8_t)(vetor[1] >> 2);
		b[3] = (uint8_t)((vetor[1] << 6) | (vetor[2] >> 7));
		b[4] = (uint8_t)((vetor[2] << 1) | (vetor[3] >> 12));
		b[5] = (uint8_t)(vetor[3] >> 4);
		b[6] = (uint8_t)((vetor[3] << 4) | (vetor[4] >> 9));
		b[7] = (uint8_t)(vetor[4] >> 1);
		b[8] = (uint8_t)((vetor[4] << 12) | (vetor[5] >> 6));
		b[9] = (uint8_t)((vetor[5] << 2) | (vetor[6] >> 11));
		b[10] = (uint8_t)(vetor[6] >> 3);
		b[11] = (uint8_t)((vetor[6] << 5) | (vetor[7] >> 8));
		b[12] = (uint8_t)vetor[7];

		if ((b[2]==0) || (b[5]==0)|| (b[7]==0)|| (b[10]==0))
		{
			tamanho_for--;
		}

		for (int i = 0; i < tamanho_for; ++i)
		{
			fwrite(&b[i] , sizeof(uint8_t) , 1, resultado);
		}
	}
	return 0 ;
}
int encontrar_x(char* c, int tamanho, int* vetor){
	int divisao, modulo, contador_vetor = 0;
	char char_divisao, char_modulo;


	for (int i = 0; i < tamanho; i = i+2)
	{
		char_divisao = c[i];
		char_modulo = c[i+1];

		for(int i =0; i< 92; i++){
			if(b91[i] == char_divisao)
				divisao = i;
			if(b91[i] == char_modulo)
				modulo = i;
		}

		vetor[contador_vetor] = (divisao*91 + modulo);
		
		contador_vetor++;
	}
	return contador_vetor;
}

void converter_para_b91(char* arquivo_entrada, char* arquivo_saida){
	FILE *arquivo, *resultado;
	uint8_t b[13];
	uint16_t c[8];
	int t, tamanho_for, n;
	int tag_igual = 1;
	// tabela

	arquivo = fopen(arquivo_entrada, "rb");
	resultado = fopen(arquivo_saida, "w");

	// arquivo = fopen("texto.txt", "rb");
	// resultado = fopen("saida.txt", "w");


	while((!feof(arquivo)) && ((t = fread(b, sizeof(uint8_t), 13, arquivo)) > 0))
	{
		int tamanho;

		tamanho = t%13;
		tamanho_for = tamanho;
		

		if (tamanho != 0)
		{
			b[t] = (b[t] & 0x00);


			if (tamanho != 12)
			{
				b[t+1] = (b[t+1] & 0x00);
			}

			if (tamanho == 12)
			{
				b[12] = 0x0D;
			}

			if ((tamanho == 3) || (tamanho == 4) || (tamanho == 5))
			{
				tamanho_for -= 1;
			}
			else if ((tamanho == 6) || (tamanho == 7))
			{
				tamanho_for -= 2;
			}
			else if ((tamanho == 8) || (tamanho == 9) || (tamanho == 10))
			{
				tamanho_for -= 3;
			}
			else if ((tamanho == 11) || (tamanho == 12))
			{
				tamanho_for -= 4;
			}
		}
		else
		{
			tamanho_for=8;
			tag_igual = 0;
		}
		

		///1 grupo de 13bits
		c[0] = (((uint16_t)b[0]) << 5 | ((uint16_t)(b[1] & 0xF8)) >> 3);

		///2 grupo de 13bits
		c[1] = (((uint16_t)(b[1] & 0x07)) << 10) | (((uint16_t)b[2]) << 2) | (((uint16_t)(b[3] & 0xC0)) >> 6);

		///3 grupo de 13bits
		c[2] = (((uint16_t)(b[3] & 0x3F)) << 7) | (((uint16_t)(b[4] & 0xFE)) >> 1);

		///4 grupo de 13bits
		c[3] = (((uint16_t)(b[4] & 0x01)) << 12) | (((uint16_t)b[5]) << 4) | (((uint16_t)(b[6] & 0xF0)) >> 4);

		///5 grupo de 13bits
		c[4] = (((uint16_t)(b[6] & 0x0F)) << 9) | (((uint16_t)b[7]) << 1) | (((uint16_t)(b[8] & 0x80)) >> 7);

		///6 grupo de 13bits
		c[5] = (((uint16_t)(b[8] & 0X7F)) << 6) | (((uint16_t)(b[9] & 0xFC)) >> 2);

		///7 grupo de 13bits
		c[6] = (((uint16_t)(b[9] & 0x03)) << 11) | (((uint16_t)b[10]) << 3) | (((uint16_t)(b[11] & 0xE0)) >> 5);

		///8 grupo de 13bits
		c[7] = (((uint16_t)(b[11] & 0x1F)) << 8) | ((uint16_t)b[12]);


		for(int i = 0; i<tamanho_for; i++){

			// if (t==12)
			// {
			// 	printf(" c->b91: %d\n", c[i]);
			// 	fwrite(&b91[(c[i]/91)] , 1 , 1, resultado);
			// 	fwrite(&b91[(c[i]%91)] , 1 , 1, resultado);
			// }
			// else if ((tamanho != 13) && (i == tamanho_for-1))
			// {
			// 	n = ((tamanho*13)%8);
			// 	n = 8191 - n;
			// 	printf(" c->b91[complemento]: %d\n", n);
			// 	//colocando o simbolo phi
			// 	fwrite(&b91[(n/91)] , 1 , 1, resultado);
			// 	fwrite(&b91[(n%91)] , 1 , 1, resultado);

			// }
			// else
			// {
				
				fwrite(&b91[(c[i]/91)] , 1 , 1, resultado);
				fwrite(&b91[(c[i]%91)] , 1 , 1, resultado);
			// }
		}
	}

	fclose(arquivo);
	fclose(resultado);
	
}

int converter(char operacao, char* arquivo_entrada, char* arquivo_saida) {
	switch(operacao) {
		case 'c':
		case 'C': 
			converter_para_b91(arquivo_entrada, arquivo_saida);
			break;
		case 'd':
		case 'D':
			converter_para_txt(arquivo_entrada, arquivo_saida);
			break;
		default:
			printf("Operacao invalida.\n");
			return 1;
	}

	return 0;
}


int main(int argc, char const *argv[]) {
	
	char arquivo_entrada[250], arquivo_saida[250], operacao;

	if(argc == 4) {
		converter(argv[1][0], (char*) argv[2], (char*) argv[3]);
	}
	else {
		printf("Operacao (C/D): ");
		scanf("%c", &operacao);
		getchar();
		printf("Nome do arquivo de entrada: ");
		scanf ("%[^\n]s", arquivo_entrada);
		getchar();
		printf("Nome do arquivo de saida: ");
		scanf ("%[^\n]s", arquivo_saida);
		getchar();

		converter(operacao, arquivo_entrada, arquivo_saida);
	}
}

