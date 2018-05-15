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
	arquivo = fopen(arquivo_entrada, "r");
	resultado = fopen(arquivo_saida, "w");
	char leitura[2];
	int sair;


	while((sair = (fread(leitura, sizeof(uint8_t), 2 , arquivo)) > 0)){
		int x = encontrar_x(leitura[0],leitura[1]);
		printf("x= %d\n", x);
		fwrite(&x , 1 , 1, resultado);
	}
	return 0 ;
}
int encontrar_x(char char_divisao, char char_modulo){
	int divisao, modulo;
	for(int i =0; i< 92; i++){
		if(b91[i] == char_divisao)
			divisao = i;
		if(b91[i] == char_modulo)
			modulo = i;
	}
	return divisao*91 + modulo;
}

int converter_para_b91(char* arquivo_entrada, char* arquivo_saida){
	FILE *arquivo, *resultado;
	uint8_t b[13];
	int c[8];
	int t;
	// tabela

	arquivo = fopen(arquivo_entrada, "r");
	resultado = fopen(arquivo_saida, "w");


	while((t = fread(b, sizeof(uint8_t), 13, arquivo)) > 0)
	{
		///1 grupo de 13bits
		c[0] = (uint32_t)(((uint16_t)b[0]) << 5 | ((uint16_t)(b[1] & 0xF8)) >> 3);

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


		for(int i = 0; i<8; i++){
			fwrite(&b91[(c[i]/91)] , 1 , 1, resultado);
			fwrite(&b91[(c[i]%91)] , 1 , 1, resultado);
		}
		
	}

	fclose(arquivo);
	fclose(resultado);

	return 0;
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
	//converter_para_b91("texto.txt", "resultado.txt");
}

