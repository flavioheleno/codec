/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "bmp.h"
#include "block.h"
#include "dct.h"
#include "quantization.h"
#include "vector.h"
#include "de.h"
#include "rle.h"
#include "file.h"
#include "codec.h"

/* sets debug mode to on */
#define DEBUG 1
/* prints messages if debug mode is on */
#define DBGMS(x) if (DEBUG) { printf(x); printf("\n"); }

/* print help options */
void print_help(const char *app) {
	printf("Usage: %s mode input output\n", app);
	printf("	modes:\n");
	printf("		-e [--encode]: encodes the given input file\n");
	printf("		-d [--decode]: decodes the given input file\n");
}

/* main function */
int main(int argc, const char *argv[]) {
	int ret;
	bmp_file bmp;
	bmp_block blocks;
	dct_data dctm;
	vector_data vector;
	FILE *f;

	/* if the number of arguments is not 4, prints help options */
	if (argc != 4)
		print_help(argv[0]);
	else {
		/* encode */
		if ((!strcmp(argv[1], "-e")) || (!strcmp(argv[1], "--encode"))) {
			DBGMS("starting encode");
			/* load bitmap file */
			ret = bmp_load(argv[2], &bmp);
			if (ret != BMP_OK) {
				bmp_error(ret);
				return 1;
			}
			DBGMS("bitmap file loaded");
			/* block creation */
			ret = block_from_bmp(&blocks, bmp);
			if (ret != BLK_OK) {
				block_error(ret);
				return 1;
			}
			DBGMS("blocks created");
			/* dct */
			ret = dct(blocks, &dctm);
			if (ret != DCT_OK) {
				dct_error(ret);
				return 1;
			}
			DBGMS("dct done");
			/* quantization */
			ret = quantization(&dctm);
			if (ret != QUA_OK) {
				quantization_error(ret);
				return 1;
			}
			DBGMS("quantization done");
			/* vectorizarion */
			ret = vector_from_block(dctm, &vector);
			if (ret != VEC_OK) {
				vector_error(ret);
				return 1;
			}
			DBGMS("vectorization done");
			/* open output file */
			f = file_open_write(argv[3]);
			if (f == NULL) {
				printf("FATAL ERROR: Can't open output file\n");
				return 1;
			}
			/* write codec header to output file */
			ret = codec_write_header(f, (uint16_t)bmp.header.width, (uint16_t)bmp.header.height);
			if (ret != COD_OK) {
				codec_error(ret);
				return 1;
			}
			DBGMS("codec header written");
			/* difference encoding */
			ret = de_encode(f, vector.dc, vector.size);
			if (ret != DE_OK) {
				de_error(ret);
				return 1;
			}
			DBGMS("de done");
			/* run-lenght encoding */
			ret = rle_encode(f, vector.data, vector.size);
			if (ret != RLE_OK) {
				rle_error(ret);
				return 1;
			}
			DBGMS("rle done");
			/* cleanup + finish */
			file_close(f);
			block_free_data(&blocks);
			bmp_free_data(&bmp);
			DBGMS("finished");
		/* decode */
		} else if ((!strcmp(argv[1], "-d")) || (!strcmp(argv[1], "--decode"))) {
			DBGMS("starting decode");
			/* opens file for reading */
			f = file_open_read(argv[2]);
			if (f == NULL) {
				printf("FATAL ERROR: Can't open input file\n");
				return 1;
			}
			/* reads the codec header */
			ret = codec_read_header(f, &bmp.header.width, &bmp.header.height);
			if (ret != COD_OK) {
				codec_error(ret);
				return 1;
			}
			DBGMS("codec header read");
			/* inits vector */
			ret = vector_init(&vector, (bmp.header.width / 8), (bmp.header.height / 8));
			if (ret != VEC_OK) {
				vector_error(ret);
				return 1;
			}
			DBGMS("vector initializated");
			/* difference encoding (decode mode) */
			ret = de_decode(f, vector.dc, vector.size);
			if (ret != DE_OK) {
				de_error(ret);
				return 1;
			}
			DBGMS("de done");
			/* run-lenght encoding (decode mode) */
			ret = rle_decode(f, vector.data, vector.size);
			if (ret != RLE_OK) {
				rle_error(ret);
				return 1;
			}
			DBGMS("rle done");
			/* dct initialization */
			ret = dct_init(&dctm, (bmp.header.width / 8), (bmp.header.height / 8));
			if (ret != DCT_OK) {
				dct_error(ret);
				return 1;
			}
			DBGMS("dct initialized");
			/* converts vector to block */
			ret = vector_to_block(vector, &dctm);
			if (ret != VEC_OK) {
				vector_error(ret);
				return 1;
			}
			DBGMS("block ready");
			/* inverse quantization */
			ret = quantization_inverse(&dctm);
			if (ret != QUA_OK) {
				quantization_error(ret);
				return 1;
			}
			DBGMS("dequantization done");
			/* inits block */
			ret = block_init(&blocks, bmp.header.width, bmp.header.height);
			if (ret != BLK_OK) {
				block_error(ret);
				return 1;
			}
			DBGMS("block initializated");
			/* inverse dct */
			ret = dct_inverse(dctm, &blocks);
			if (ret != DCT_OK) {
				dct_error(ret);
				return 1;
			}
			DBGMS("idct done");
			/* converts block to bmp format */
			ret = block_to_bmp(blocks, &bmp);
			if (ret != BLK_OK) {
				block_error(ret);
				return 1;
			}
			DBGMS("bmp rebuild");
			/* writes bmp data to file */
			ret = bmp_save(argv[3], bmp);
			if (ret != BMP_OK) {
				bmp_error(ret);
				return 1;
			}
			DBGMS("bmp done");
			file_close(f);
			DBGMS("finished");
		/* invalid option */
		} else {
			print_help(argv[0]);
		}
	}
	return 0;
}
