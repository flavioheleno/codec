/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "vector.h"

void vector_print(vector_data data) {
	int i, j;
	printf("Vector RED:\n");
	for (i = 0; i < data.size; i++) {
		printf("%d:\t", i);
		for (j = 0; j < 64; j++)
			printf("%d ", data.data[i][j].r);
		printf("\n");
	}
	printf("Vector BLUE:\n");
	for (i = 0; i < data.size; i++) {
		printf("%d:\t", i);
		for (j = 0; j < 64; j++)
			printf("%d ", data.data[i][j].b);
		printf("\n");
	}
	printf("Vector GREEN:\n");
	for (i = 0; i < data.size; i++) {
		printf("%d:\t", i);
		for (j = 0; j < 64; j++)
			printf("%d ", data.data[i][j].g);
		printf("\n");
	}
}

int vector_init(vector_data *target, uint32_t width, uint32_t height) {
	int i;
	target->size = (width * height);
	target->data = (pixel_int32 **)malloc(sizeof(pixel_int32 *) * target->size);
	if (target->data == NULL)
		return VEC_MERR;
	for (i = 0; i < target->size; i++) {
		target->data[i] = (pixel_int32 *)malloc(sizeof(pixel_int32) * 64);
		if (target->data[i] == NULL)
			return VEC_MERR;
	}
	target->dc = (pixel_int32 *)malloc(sizeof(pixel_int32) * target->size);
	if (target->dc == NULL)
		return VEC_MERR;
	return VEC_OK;
}

/* algorithm taken from http://rosettacode.org/wiki/Zig_Zag#C */
void vector_scan(pixel_int32 source[8][8], pixel_int32 target[64]) {
	int i, x = 1, y = 1;
	for (i = 0; i < 64; i++) {
		target[i].r = (int32_t)source[(x - 1)][(y - 1)].r;
		target[i].g = (int32_t)source[(x - 1)][(y - 1)].g;
		target[i].b = (int32_t)source[(x - 1)][(y - 1)].b;
		if (((x + y) % 2) == 0) {
			if (y < 8)
				y++;
			else
				x += 2;
			if (x > 1)
				x--;
		} else {
			if (x < 8)
				x++;
			else
				y += 2;
			if (y > 1)
				y--;
		}
	}
}

/* algorithm taken from http://rosettacode.org/wiki/Zig_Zag#C */
void vector_scan_reverse(pixel_int32 source[64], pixel_int32 target[8][8]) {
	int i, x = 1, y = 1;
	for (i = 0; i < 64; i++) {
		target[(x - 1)][(y - 1)].r = (int8_t)source[i].r;
		target[(x - 1)][(y - 1)].g = (int8_t)source[i].g;
		target[(x - 1)][(y - 1)].b = (int8_t)source[i].b;
		if (((x + y) % 2) == 0) {
			if (y < 8)
				y++;
			else
				x += 2;
			if (x > 1)
				x--;
		} else {
			if (x < 8)
				x++;
			else
				y += 2;
			if (y > 1)
				y--;
		}
	}
}

int vector_from_block(dct_data block, vector_data *target) {
	int x, y, i, ret;
	ret = vector_init(target, block.width, block.height);
	if (ret != VEC_OK)
		return ret;
	i = 0;
	for (x = 0; x < block.width; x++)
		for (y = 0; y < block.height; y++) {
			/* zig-zag scan */
			vector_scan(block.block[x][y].data, target->data[i]);
			/* to dc coefficients */
			target->dc[i].r = (int32_t)block.block[x][y].data[0][0].r;
			target->dc[i].g = (int32_t)block.block[x][y].data[0][0].g;
			target->dc[i].b = (int32_t)block.block[x][y].data[0][0].b;
			i++;
		}
	return VEC_OK;
}

int vector_to_block(vector_data source, dct_data *block) {
	int x, y, i;
	i = 0;
	for (x = 0; x < block->width; x++)
		for (y = 0; y < block->height; y++) {
			/* zig-zag scan (reverse mode) */
			vector_scan_reverse(source.data[i], block->block[x][y].data);
			/* from dc coefficients */
			block->block[x][y].data[0][0].r = (int8_t)source.dc[i].r;
			block->block[x][y].data[0][0].g = (int8_t)source.dc[i].g;
			block->block[x][y].data[0][0].b = (int8_t)source.dc[i].b;
			i++;
		}
	return VEC_OK;
}


void vector_error(int code) {
	switch (code) {
		case VEC_MERR:
			printf("VECTOR: malloc error\n");
			break;
	}
}
