/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "bmp.h"

/* defines for pseudo helper function */
#define error_handler(f, v)	{ if (f) fclose(f); return v; }

void bmp_print_header(bmp_header header) {
	printf("(FILE HEADER)\n");
	printf("	File size:		%08X (%u)\n", header.fsize, header.fsize);
	printf("	Reserved:		%08X (%u)\n", header.reserved, header.reserved);
	printf("	Offset:			%08X (%u)\n", header.offset, header.offset);
	printf("(INFO HEADER)\n");
	printf("	Header size:		%08X (%u)\n", header.hsize, header.hsize);
	printf("	Width:			%08X (%u)\n", header.width, header.width);
	printf("	Height:			%08X (%u)\n", header.height, header.height);
	printf("	Planes:			%08X (%u)\n", header.planes, header.planes);
	printf("	Bits per pixel:		%08X (%u)\n", header.bpp, header.bpp);
	printf("	Compression:		%08X (%u)\n", header.compression, header.compression);
	printf("	Data size:		%08X (%u)\n", header.dsize, header.dsize);
	printf("	Horizontal resolution:	%08X (%u)\n", header.hres, header.hres);
	printf("	Vertical resolution:	%08X (%u)\n", header.vres, header.vres);
	printf("	Colors:			%08X (%u)\n", header.colors, header.colors);
	printf("	Important:		%08X (%u)\n", header.important, header.important);
}

void bmp_print_data(bmp_file bmp) {
	int x, y;
	printf("(DATA)\n");
	for (y = 0; y < bmp.header.height; y++) {
		for (x = 0; x < bmp.header.width; x++)
			printf("%02X%02X%02X  |  ", bmp.data[x][y].r, bmp.data[x][y].g, bmp.data[x][y].b);
		printf("\n");
	}
}

void bmp_free_data(bmp_file *bmp) {
	int i;
	/* for each line, free its memory */
	for (i = 0; i < bmp->header.width; i++)
		free(bmp->data[i]);
	/* free initial vector */
	free(bmp->data);
}

int bmp_load(const char *filename, bmp_file *bmp) {
	FILE *file = NULL;
	uint32_t bytes_read = 0; /* counts the number of bytes already read */
	uint8_t padding = 0; /* size of the padding used for alignment */
	uint8_t count = 0; /* counts the number of bytes until padding */
	uint16_t bmp_id;
	int i, x, y; /* auxiliar variables */
	file = fopen(filename, "rb"); /* opens file for binary reading */
	if (!file)
		return BMP_FERR;
	/* reads bmp id number */
	if (fread(&bmp_id, sizeof(bmp_id), (size_t)1, file) != 1)
		error_handler(file, BMP_RERR);
	/* checks bmp id */
	if (bmp_id != 0x4D42)
		error_handler(file, BMP_IFF);
	/* reads bmp header */
	if (fread(&bmp->header, sizeof(bmp_header), (size_t)1, file) != 1)
		error_handler(file, BMP_RERR);
	/* calculates the padding used for alignment (4 byte alignment) */
	padding = (bmp->header.width % 4);
	/* sets the number of bytes to read */
	bytes_read = bmp->header.dsize;
	/* seeks the file pointer to offset position (start of data) */
	if (fseek(file, bmp->header.offset, SEEK_SET))
		error_handler(file, BMP_SERR);
	/* memory allocation for pixel map */
	bmp->data = (pixel_uint8 **)malloc(sizeof(pixel_uint8 *) * bmp->header.width);
	if (bmp->data == NULL)
		error_handler(file, BMP_MERR);
	for (i = 0; i < bmp->header.width; i++) {
		bmp->data[i] = (pixel_uint8 *)malloc(sizeof(pixel_uint8) * bmp->header.height);
		if (bmp->data[i] == NULL)
			error_handler(file, BMP_MERR);
	}
	/* bmp pixels are stor in reverse order for y axis and normal order for x axis */
	x = 0;
	y = (bmp->header.height - 1);
	/* while there are bytes to be read and the file isn't in the end and neither are file errors */
	while ((bytes_read > 0) && (!feof(file)) && (!ferror(file))) {
		/* read 3 bytes (one for r, one for g and one for b) */
		bytes_read -= 3;
		/* reads pixel from file */
		if (fread(&bmp->data[x][y], sizeof(pixel_uint8), (size_t)1, file) != 1) {
			bmp_free_data(bmp);
			error_handler(file, BMP_RERR);
		}

		/* if there is padding to align the pixels */
		if (padding) {
			/* increases the counting */
			count++;
			/* if the number of read bytes is equal the width of the image, padding must be jumped */
			if (count == bmp->header.width) {
				/* decrements the padding size of the data size counter */
				bytes_read -= padding;
				count = 0;
				/* seeks the file pointer to jump the padding size */
				if (fseek(file, padding, SEEK_CUR)) {
					bmp_free_data(bmp);
					error_handler(file, BMP_SERR);
				}
			}
		}

		/* updates x and y axis for pixel storage */
		x++;
		if (x == bmp->header.width) {
			y--;
			x = 0;
		}
	}
	/* closes bmp file */
	fclose(file);
	return BMP_OK;
}

int bmp_save(const char *filename, bmp_file bmp) {
	FILE *file = NULL;
	static uint16_t bmp_id = 0x4D42;
	int x, y;
	file = fopen(filename, "wb");
	if (!file)
		return BMP_FERR;
	/* writes bmp id number */
	if (fwrite(&bmp_id, sizeof(bmp_id), (size_t)1, file) != 1)
		error_handler(file, BMP_WERR);
	/* writes bmp header */
	if (fwrite(&bmp.header, sizeof(bmp_header), (size_t)1, file) != 1)
		error_handler(file, BMP_WERR);
	/* writes data to file */
	for (y = (bmp.header.height - 1); y >= 0; y--)
		for (x = 0; x < bmp.header.width; x++) {
			if (fwrite(&bmp.data[x][y], sizeof(pixel_uint8), (size_t)1, file) != 1)
				error_handler(file, BMP_WERR);
		}
	fclose(file);
	return BMP_OK;
}

void bmp_error(int code) {
	switch (code) {
		case BMP_FERR:
			printf("BMP: file error\n");
			break;
		case BMP_RERR:
			printf("BMP: read error\n");
			break;
		case BMP_IFF:
			printf("BMP: invalid file format\n");
			break;
		case BMP_SERR:
			printf("BMP: seek error\n");
			break;
		case BMP_MERR:
			printf("BMP: malloc error\n");
			break;
	}
}
