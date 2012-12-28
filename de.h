/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __DE_H__
#define __DE_H__

#include <stdint.h>

#include "pixel.h"
#include "binary.h"
#include "huffman.h"
#include "file.h"

/* defines return values */
#define DE_OK 0x00
#define DE_MERR 0x01
#define DE_HERR 0x02
#define DE_WERR 0x03
#define DE_RERR 0x04
#define DE_FERR 0x05

/* defines structure to hold de data */
typedef struct {
	int32_t diff;
	uint8_t diff_bits;
	uint32_t code;
	uint8_t code_bits;
} de_table;

/*
	This function prints the table used for de
	Parameters:
		table: the table
		size: table size (num of lines)
	Return values:
		none
*/
void de_print(de_table *table, int size);

/*
	This function does difference encoding
	Parameters:
		handler: output file pointer
		vector: pixel data to be encoded
		size: size of vector array
	Return values:
		DE_OK: encode ok
		DE_MERR: malloc error
		DE_WERR: write error
*/
int de_encode(FILE *handler, pixel_int32 *vector, int size);

/*
	This function does difference decoding
	Parameters:
		handler: input file pointer
		vector: pixel data decoded
		size: size of vector array
	Return values:
		DE_OK: decode ok
		DE_MERR: malloc error
		DE_RERR: read error
*/
int de_decode(FILE *handler, pixel_int32 *vector, int size);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void de_error(int code);

#endif
