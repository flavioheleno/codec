/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "quantization.h"

int quantization(dct_data *dct) {
	int i, j, x, y;
	/* quantization matrix */
	int matrix[8][8] = {
		{16, 11, 10, 16, 24,  40,  51,  61},
		{12, 12, 14, 19, 26,  58,  60,  55},
		{14, 13, 16, 24, 40,  57,  69,  56},
		{14, 17, 22, 29, 51,  87,  80,  62},
		{18, 22, 37, 56, 68,  109, 103, 77},
		{24, 35, 55, 64, 81,  104, 113, 92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92, 95, 98, 112, 100, 103, 99}
	};
	for (i = 0; i < dct->width; i++)
		for (j = 0; j < dct->height; j++)
			for (x = 0; x < 8; x++)
				for (y = 0; y < 8; y++) {
					dct->block[i][j].data[x][y].r /= matrix[x][y];
					dct->block[i][j].data[x][y].g /= matrix[x][y];
					dct->block[i][j].data[x][y].b /= matrix[x][y];
				}
	return QUA_OK;
}

int quantization_inverse(dct_data *dct) {
	int i, j, x, y;
	/* quantization matrix */
	int matrix[8][8] = {
		{16, 11, 10, 16, 24,  40,  51,  61},
		{12, 12, 14, 19, 26,  58,  60,  55},
		{14, 13, 16, 24, 40,  57,  69,  56},
		{14, 17, 22, 29, 51,  87,  80,  62},
		{18, 22, 37, 56, 68,  109, 103, 77},
		{24, 35, 55, 64, 81,  104, 113, 92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92, 95, 98, 112, 100, 103, 99}
	};
	for (i = 0; i < dct->width; i++)
		for (j = 0; j < dct->height; j++)
			for (x = 0; x < 8; x++)
				for (y = 0; y < 8; y++) {
					dct->block[i][j].data[x][y].r *= matrix[x][y];
					dct->block[i][j].data[x][y].g *= matrix[x][y];
					dct->block[i][j].data[x][y].b *= matrix[x][y];
				}
	return QUA_OK;
}

void quantization_error(int code) {
	printf("QUA: error code %d\n", code);
}
