#include <stdlib.h>
#include <stdio.h>
#include <string.h>








int main(int argc, char const *argv[])
{
	FILE *arquivo, *resultado;
	uint8_t b[13];
	int c[8];
	int t;
	// tabela
	const char b91[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "abcdefghijklmnopqrstuvwxyz"
                      "0123456789"
                      "!#$>?@[]^_`{|}~\"%&()*+,./:;<=" ;

	arquivo = fopen("arquivo.txt", "r");
	resultado = fopen("resultado.txt", "w");


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


		printf("%d\n", c[0]);
		printf("%d\n", c[1]);
		printf("%d\n", c[2]);
		printf("%d\n", c[3]);
		printf("%d\n", c[4]);
		printf("%d\n", c[5]);
		printf("%d\n", c[6]);
		printf("%d\n", c[7]);


		fwrite(&b91[(c[0]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[0]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[1]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[1]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[2]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[2]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[3]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[3]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[4]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[4]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[5]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[5]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[6]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[6]%91)] , 1 , 1, resultado);

		fwrite(&b91[(c[7]/91)] , 1 , 1, resultado);
		fwrite(&b91[(c[7]%91)] , 1 , 1, resultado);
	}

	fclose(arquivo);
	fclose(resultado);

	// printf("%s\n", b);

	// t = strlen(vetor);

	// printf("%d\n", t);


	return 0;
}