/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "dct.h"

/* defines PI constant */
#define PI 3.1415926535
/* pseudo helper function */
#define C(x) ((x == 0) ? 0.707106781 : 1)

/*
	This function applies the dct to the given blocks
	Parameters:
		source: block to be applied the dct
		target: variable to store the result of the dct
	Return values:
		DCT_OK: inverse dct ok
*/
int block_dct(data_block source, dct_block *target) {
	int i, j, x, y;
	float a, b;
	float sum[3];
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			sum[0] = 0.0;
			sum[1] = 0.0;
			sum[2] = 0.0;
			for (x = 0; x < 8; x++)
				for (y = 0; y < 8; y++) {
					a = ((((2 * x) + 1) * i * PI) / 16);
					b = ((((2 * y) + 1) * j * PI) / 16);
					sum[0] += (source.data[x][y].r * cos(a) * cos(b));
					sum[1] += (source.data[x][y].g * cos(a) * cos(b));
					sum[2] += (source.data[x][y].b * cos(a) * cos(b));
				}
			target->data[i][j].r = (0.25 * C(i) * C(j) * sum[0]);
			target->data[i][j].g = (0.25 * C(i) * C(j) * sum[1]);
			target->data[i][j].b = (0.25 * C(i) * C(j) * sum[2]);
		}
	return DCT_OK;
}

/*
	This function applies the inverse dct to the given blocks
	Parameters:
		source: block to be applied the inverse dct
		target: variable to store the result of the inverse dct
	Return values:
		DCT_OK: inverse dct ok
*/
int block_dct_inverse(dct_block source, data_block *target) {
	int i, j, x, y;
	float a, b;
	float sum[3];
	for (x = 0; x < 8; x++)
		for (y = 0; y < 8; y++) {
			sum[0] = 0.0;
			sum[1] = 0.0;
			sum[2] = 0.0;
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++) {
					a = ((((2 * x) + 1) * i * PI) / 16);
					b = ((((2 * y) + 1) * j * PI) / 16);
					sum[0] += (C(i) * C(j) * source.data[i][j].r * cos(a) * cos(b));
					sum[1] += (C(i) * C(j) * source.data[i][j].g * cos(a) * cos(b));
					sum[2] += (C(i) * C(j) * source.data[i][j].b * cos(a) * cos(b));
				}
			target->data[x][y].r = (0.25 * sum[0]);
			target->data[x][y].g = (0.25 * sum[1]);
			target->data[x][y].b = (0.25 * sum[2]);
		}
	return DCT_OK;
}

int dct_init(dct_data *target, int width, int height) {
	int i;
	dct_block **tmp;
	tmp = (dct_block **)malloc(sizeof(dct_block *) * width);
	if (tmp == NULL)
		return DCT_MERR;
	for (i = 0; i < width; i++) {
		tmp[i] = (dct_block *)malloc(sizeof(dct_block) * height);
		if (tmp[i] == NULL)
			return DCT_MERR;
	}
	target->width = width;
	target->height = height;
	target->block = tmp;
	return DCT_OK;
}

int dct(bmp_block source, dct_data *target) {
	int i, j, ret;
	ret = dct_init(target, source.width, source.height);
	if (ret != DCT_OK)
		return ret;
	for (i = 0; i < source.width; i++)
		for (j = 0; j < source.height; j++)
			block_dct(source.block[i][j], &target->block[i][j]);
	return DCT_OK;
}

int dct_inverse(dct_data source, bmp_block *target) {
	int i, j;
	data_block **tmp;
	tmp = (data_block **)malloc(sizeof(data_block *) * source.width);
	if (tmp == NULL)
		return DCT_MERR;
	for (i = 0; i < source.width; i++) {
		tmp[i] = (data_block *)malloc(sizeof(data_block) * source.height);
		if (tmp[i] == NULL)
			return DCT_MERR;
	}
	for (i = 0; i < source.width; i++)
		for (j = 0; j < source.height; j++)
			block_dct_inverse(source.block[i][j], &tmp[i][j]);
	target->width = source.width;
	target->height = source.height;
	target->block = tmp;
	return DCT_OK;
}

void dct_error(int code) {
	switch (code) {
		case DCT_MERR:
			printf("DCT: malloc error\n");
			break;
	}
}
