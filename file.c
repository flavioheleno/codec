/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "file.h"

/* defines for pseudo helper functions */
#define error_handler(f, v)	{ if (f) fclose(f); return v; }
#define pos(x, y, w) (x + (y * w))
#define idx(x, y) ((x * 3) + (y * 24))

int file_write(const char *filename, bmp_block blocks) {
	FILE *file = NULL;
	/* buffer used to write data to file */
	uint32_t buffer = 0x00000000;
	/* number of bits used to represent int8_t data on reduced representation */
	int size = 0x00;
	/* auxiliar variable that holds converted data into reduced representation */
	uint8_t temp = 0x00;
	/* rotation counter, will be used to control buffer usage */
	uint8_t rot = 0x00;
	/* binary mask */
	uint8_t mask = 0x00;
	int w, h, x, y;
	/* header id for file format check */
	static uint32_t header = 0x21424846;

	file = fopen(filename, "wb");
	if (!file)
		return FILE_FERR;

	if (fwrite(&header, sizeof(uint32_t), (size_t)1, file) != 1)
		error_handler(file, FILE_WERR);

	/* for each block (w x h) */
	for (w = 0; w < blocks.width; w++)
		for (h = 0; h < blocks.height; h++)
			/* for each block pixel (8 x 8) */
			for (x = 0; x < 8; x++)
				for (y = 0; y < 8; y++) {
					/* red data */
					/* convert int8_t to our representation */
					temp = binary_reduce8(blocks.block[w][h].data[x][y].r);
					/* calculates the number of bits used to represent int8_t data */
					size = binary_size8(blocks.block[w][h].data[x][y].r);
					/* creates binary mask */
					mask = binary_mask8(size);
					/* if buffer size is enough to store current data */
					if ((rot + size) <= 0x20) {
						/* rotates buffer data to left by size bits */
						buffer <<= size;
						/* logical or to store current data on buffer */
						buffer |= (uint32_t)(temp & mask);
						/* updates rotation count */
						rot += size;
					/* there is not enough size on buffer, so bufferizes what can be bufferized, print to file and restarts buffer */
					} else {
						/* rotates buffer data to left by remaining bits */
						buffer <<= (0x20 - rot);
						/* updates data size with the number of remaining bits */
						size -= (0x20 - rot);
						/* logical or to store the first bits of current data on buffer */
						buffer |= (uint32_t)((temp & mask) >> size);
						/* prints buffer contents to file */
						if (fwrite(&buffer, sizeof(uint32_t), (size_t)1, file) != 1)
							error_handler(file, FILE_WERR);
						/* cleans buffer */
						buffer = 0x00000000;
						/* updates rotation count with remaining data size */
						rot = size;
						/*
							logical or to store the remaining bits on buffer
							note that MSB aren't stripped from data because rotation counter will do that later
						 */
						buffer |= (uint32_t)(temp & mask);
					}
					/* green data */
					temp = binary_reduce8(blocks.block[w][h].data[x][y].g);
					size = binary_size8(blocks.block[w][h].data[x][y].g);
					mask = binary_mask8(size);
					if ((rot + size) <= 0x20) {
						buffer <<= size;
						buffer |= (uint32_t)(temp & mask);
						rot += size;
					} else {
						buffer <<= (0x20 - rot);
						size -= (0x20 - rot);
						buffer |= (uint32_t)((temp & mask) >> size);
						if (fwrite(&buffer, sizeof(uint32_t), (size_t)1, file) != 1)
							error_handler(file, FILE_WERR);
						buffer = 0x00000000;
						rot = size;
						buffer |= (uint32_t)(temp & mask);
					}
					/* blue data */
					temp = binary_reduce8(blocks.block[w][h].data[x][y].b);
					size = binary_size8(blocks.block[w][h].data[x][y].b);
					mask = binary_mask8(size);
					if ((rot + size) <= 0x20) {
						buffer <<= size;
						buffer |= (uint32_t)(temp & mask);
						rot += size;
					} else {
						buffer <<= (0x20 - rot);
						size -= (0x20 - rot);
						buffer |= (uint32_t)((temp & mask) >> size);
						if (fwrite(&buffer, sizeof(uint32_t), (size_t)1, file) != 1)
							error_handler(file, FILE_WERR);
						buffer = 0x00000000;
						rot = size;
						buffer |= (uint32_t)(temp & mask);
					}
				}

	/* if data size is not aligned to 20h, fills the buffer and writes to file */
	if (rot) {
		buffer <<= (0x20 - rot);
		if (fwrite(&buffer, sizeof(uint32_t), (size_t)1, file) != 1)
			error_handler(file, FILE_WERR);
	}

	fclose(file);
	return FILE_OK;
}

int file_read(const char *filename, bmp_block *blocks, uint8_t **table, uint32_t width, uint32_t height) {
	FILE *file = NULL;
	uint32_t header;
	uint32_t *buffer = NULL;
	uint32_t mask = 0x00000000;
	uint8_t temp;
	int fpos, size, i, w, h, x, y, bits, pos = 0, rot = 0;
	blocks->width = width;
	blocks->height = height;

	file = fopen(filename, "rb");
	if (!file)
		return FILE_FERR;
	if (fread(&header, sizeof(uint32_t), (size_t)1, file) != 1)
		error_handler(file, FILE_RERR);
	if (header != 0x21424846)
		error_handler(file, FILE_IFF);

	/* memory allocation for block data */
	blocks->block = (data_block **)malloc(sizeof(data_block *) * blocks->width);
	if (blocks->block == NULL)
		error_handler(file, FILE_MERR);
	for (i = 0; i < blocks->width; i++) {
		blocks->block[i] = (data_block *)malloc(sizeof(data_block) * blocks->height);
		if (blocks->block[i] == NULL)
			error_handler(file, FILE_MERR);
	}

	/* calculating buffer size */
	fpos = ftell(file);
	if (fseek(file, 0, SEEK_END))
		error_handler(file, FILE_SERR);
	size = ((ftell(file) - fpos) / 4);
	if (fseek(file, fpos, SEEK_SET))
		error_handler(file, FILE_SERR);

	/* memory allocation for buffer */
	buffer = (uint32_t *)malloc(sizeof(uint32_t) * size);
	if (buffer == NULL)
		error_handler(file, FILE_MERR);

	/* reads file contents */
	if (fread(buffer, sizeof(uint32_t), (size_t)size, file) != size)
		error_handler(file, FILE_RERR);

	/* foreach block */
	for (w = 0; w < blocks->width; w++)
		for (h = 0; h < blocks->height; h++)
			for (x = 0; x < 8; x++)
				for (y = 0; y < 8; y++) {
					/* red info */
					mask = 0x00000000;
					/* number of bits to be used */
					bits = table[pos(w, h, blocks->width)][idx(x, y)];
					/* if the data is in the current buffer item */
					if ((rot + bits) < 0x20) {
						/* creates bit mask */
						for (i = bits; i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						/* increment rotation counter with the number of bits that will be readed */
						rot += bits;
						/* rotates buffer item, applies mask and then expand content */
						blocks->block[w][h].data[x][y].r = binary_expand8((uint8_t)((buffer[pos] >> (0x20 - rot)) & mask), (uint8_t)bits);
					/* data is half in the current buffer item and half on the next item */
					} else {
						/* creates bit mask */
						for (i = (0x20 - rot); i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						/* loads current item data into temporary buffer */
						temp = (uint8_t)(buffer[pos] & mask);
						/* updates bits number with the left bits */
						bits -= (0x20 - rot);
						/* rotates temporary buffer to receive the next bits */
						temp <<= bits;
						/* creates bit mask */
						mask = 0x00000000;
						for (i = bits; i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						/* updates rotation counter with the number of bits that will be readed */
						rot = bits;
						/* moves to next buffer item */
						pos++;
						/* rotates buffer item, applies mask and then stores into temporary buffer */
						temp |= (uint8_t)((buffer[pos] >> (0x20 - rot)) & mask);
						/* expand temporary buffer */
						blocks->block[w][h].data[x][y].r = binary_expand8(temp, table[pos(w, h, blocks->width)][idx(x, y)]);
					}
					/* green info */
					mask = 0x00000000;
					bits = table[pos(w, h, blocks->width)][idx(x, y) + 1];
					if ((rot + bits) < 0x20) {
						for (i = bits; i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						rot += bits;
						blocks->block[w][h].data[x][y].g = binary_expand8((uint8_t)((buffer[pos] >> (0x20 - rot)) & mask), (uint8_t)bits);
					} else {
						for (i = (0x20 - rot); i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						temp = (uint8_t)(buffer[pos] & mask);
						bits -= (0x20 - rot);
						temp <<= bits;
						mask = 0x00000000;
						for (i = bits; i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						rot = bits;
						pos++;
						temp |= (uint8_t)((buffer[pos] >> (0x20 - rot)) & mask);
						blocks->block[w][h].data[x][y].g = binary_expand8(temp, table[pos(w, h, blocks->width)][idx(x, y) + 1]);
					}
					/* blue info */
					mask = 0x00000000;
					bits = table[pos(w, h, blocks->width)][idx(x, y) + 2];
					if ((rot + bits) < 0x20) {
						for (i = bits; i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						rot += bits;
						blocks->block[w][h].data[x][y].b = binary_expand8((uint8_t)((buffer[pos] >> (0x20 - rot)) & mask), (uint8_t)bits);
					} else {
						for (i = (0x20 - rot); i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						temp = (uint8_t)(buffer[pos] & mask);
						bits -= (0x20 - rot);
						temp <<= bits;
						mask = 0x00000000;
						for (i = bits; i > 0; i--) {
							mask <<= 1;
							mask |= 0x01;
						}
						rot = bits;
						pos++;
						temp |= (uint8_t)((buffer[pos] >> (0x20 - rot)) & mask);
						blocks->block[w][h].data[x][y].b = binary_expand8(temp, table[pos(w, h, blocks->width)][idx(x, y) + 2]);
					}
				}

	free(buffer);
	fclose(file);
	return FILE_OK;
}

FILE *file_open_write(const char *filename) {
	return fopen(filename, "wb");
}

FILE *file_open_read(const char *filename) {
	return fopen(filename, "rb");
}

int file_close(FILE *handler) {
	if (handler == NULL)
		return FILE_FERR;
	fclose(handler);
	return FILE_OK;
}

int file_write8(FILE *handler, uint8_t data) {
	if (handler == NULL)
		return FILE_FERR;
	if (fwrite(&data, sizeof(uint8_t), (size_t)1, handler) == 1)
		return FILE_OK;
	return FILE_WERR;
}

int file_write16(FILE *handler, uint16_t data) {
	if (handler == NULL)
		return FILE_FERR;
	if (fwrite(&data, sizeof(uint16_t), (size_t)1, handler) == 1)
		return FILE_OK;
	return FILE_WERR;
}

int file_write32(FILE *handler, uint32_t data) {
	if (handler == NULL)
		return FILE_FERR;
	if (fwrite(&data, sizeof(uint32_t), (size_t)1, handler) == 1)
		return FILE_OK;
	return FILE_WERR;
}

int file_write_buffered(FILE *handler, uint32_t data, uint8_t bits, int flush) {
	static uint32_t buffer = 0;
	static uint8_t size = 0;
	uint32_t mask = 0;
	/*printf("write: %s (%u)\n", binary_print32(data), bits);*/
	if (flush) {
		buffer <<= (0x20 - size);
		size = 0;
		return file_write32(handler, buffer);
	} else {
		mask = binary_mask32(bits);
		if ((size + bits) <= 0x20) {
			buffer <<= bits;
			buffer |= (uint32_t)(data & mask);
			size += bits;
		} else {
			buffer <<= (0x20 - size);
			bits -= (0x20 - size);
			buffer |= (uint32_t)((data & mask) >> bits);
			if (file_write32(handler, buffer) != FILE_OK)
				return FILE_WERR;
			buffer = 0x00000000;
			size = bits;
			buffer |= (uint32_t)(data & mask);
		}
	}
	return FILE_OK;
}

int file_read8(FILE *handler, uint8_t *data) {
	if (handler == NULL)
		return FILE_FERR;
	if (fread(data, sizeof(uint8_t), (size_t)1, handler) == 1)
		return FILE_OK;
	return FILE_RERR;
}

int file_read16(FILE *handler, uint16_t *data) {
	if (handler == NULL)
		return FILE_FERR;
	if (fread(data, sizeof(uint16_t), (size_t)1, handler) == 1)
		return FILE_OK;
	return FILE_RERR;
}

int file_read32(FILE *handler, uint32_t *data) {
	if (handler == NULL)
		return FILE_FERR;
	if (fread(data, sizeof(uint32_t), (size_t)1, handler) == 1)
		return FILE_OK;
	return FILE_RERR;
}

int file_read_buffered(FILE *handler, uint32_t **data, uint16_t size) {
	int i, ret;
	uint32_t *buffer;
	if (handler == NULL)
		return FILE_FERR;
	buffer = (uint32_t *)malloc(sizeof(uint32_t) * size);
	if (buffer == NULL)
		return FILE_MERR;
	for (i = 0; i < size; i++) {
		ret = file_read32(handler, &buffer[i]);
		if (ret != FILE_OK)
			return ret;
	}
	*data = buffer;
	return FILE_OK;
}
