/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "block.h"

/* defines the filter value */
#define FILTER 0x80

/* defines for pseudo helper function */
#define pos1(v) (int)floor(v / 8)
#define pos2(a, b) ((a * 8) + b)
#define idx1(v) (v % 8)

int block_init(bmp_block *blocks, int width, int height) {
	int i;
	/* calculates block width and height */
	blocks->width = (uint32_t)(width / 8);
	blocks->height = (uint32_t)(height / 8);
	/* memory allocation for block data */
	blocks->block = (data_block **)malloc(sizeof(data_block *) * blocks->width);
	if (blocks->block == NULL)
		return BLK_MERR;
	for (i = 0; i < blocks->width; i++) {
		blocks->block[i] = (data_block *)malloc(sizeof(data_block) * blocks->height);
		if (blocks->block[i] == NULL)
			return BLK_MERR;
	}
	return BLK_OK;
}

void block_free_data(bmp_block *blocks) {
	int i;
	/* for each line, free its memory */
	for (i = 0; i < blocks->width; i++)
		free(blocks->block[i]);
	/* free initial vector */
	free(blocks->block);
}

int block_from_bmp(bmp_block *blocks, bmp_file bmp) {
	int x, y, ret;

	ret = block_init(blocks, bmp.header.width, bmp.header.height);
	if (ret != BLK_OK)
		return ret;

	/* foreach pixel, stores pixel data (applying level shift) */
	for (x = 0; x < bmp.header.width; x++)
		for (y = 0; y < bmp.header.height; y++) {
			blocks->block[pos1(x)][pos1(y)].data[idx1(x)][idx1(y)].r = ((int8_t)bmp.data[x][y].r - FILTER);
			blocks->block[pos1(x)][pos1(y)].data[idx1(x)][idx1(y)].g = ((int8_t)bmp.data[x][y].g - FILTER);
			blocks->block[pos1(x)][pos1(y)].data[idx1(x)][idx1(y)].b = ((int8_t)bmp.data[x][y].b - FILTER);
		}
	return BLK_OK;
}

int block_to_bmp(bmp_block blocks, bmp_file *bmp) {
	int i, w, h, x, y;
	uint32_t size = (blocks.width * blocks.height * 192);

	/* recreates bitmap header */
	bmp->header.fsize = (size + 0x00000036);
	bmp->header.reserved = 0x00000000;
	bmp->header.offset = 0x00000036;
	bmp->header.hsize = 0x00000028;
	bmp->header.width = (blocks.width * 8);
	bmp->header.height = (blocks.height * 8);
	bmp->header.planes = 0x0001;
	bmp->header.bpp = 0x0018;
	bmp->header.compression = 0x00000000;
	bmp->header.dsize = size;
	bmp->header.hres = 0x00000B13;
	bmp->header.vres = 0x00000B13;
	bmp->header.colors = 0x00000000;
	bmp->header.important = 0x00000000;

	/* memory allocation for bitmap data */
	bmp->data = (pixel_uint8 **)malloc(sizeof(pixel_uint8 *) * bmp->header.width);
	if (bmp->data == NULL)
		return BLK_MERR;
	for (i = 0; i < bmp->header.width; i++) {
		bmp->data[i] = (pixel_uint8 *)malloc(sizeof(pixel_uint8) * bmp->header.height);
		if (bmp->data[i] == NULL)
			return BLK_MERR;
	}

	/* foreach block, loads every pixel data (applying level shift) */
	for (w = 0; w < blocks.width; w++)
		for (h = 0; h < blocks.height; h++)
			for (x = 0; x < 8; x++)
				for (y = 0; y < 8; y++) {
					bmp->data[pos2(w, x)][pos2(h, y)].r = (uint8_t)(blocks.block[w][h].data[x][y].r + FILTER);
					bmp->data[pos2(w, x)][pos2(h, y)].g = (uint8_t)(blocks.block[w][h].data[x][y].g + FILTER);
					bmp->data[pos2(w, x)][pos2(h, y)].b = (uint8_t)(blocks.block[w][h].data[x][y].b + FILTER);
				}

	return BLK_OK;
}

void block_error(int code) {
	switch (code) {
		case BLK_MERR:
			printf("BLOCK: malloc error\n");
			break;
	}
}
