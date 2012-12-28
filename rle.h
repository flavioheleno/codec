/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __RLE_H__
#define __RLE_H__

#include <stdio.h>
#include <stdlib.h>

#include "pixel.h"
#include "file.h"
#include "huffman.h"

/* defines return values */
#define RLE_OK 0x00
#define RLE_MERR 0x01
#define RLE_WERR 0x02
#define RLE_FERR 0x03
#define RLE_RERR 0x04
#define RLE_HERR 0x05

/* defines structure to hold rle pre-table */
typedef struct {
	uint8_t repeat;
	int32_t value;
} rle;

typedef struct {
	rle *info[3];
	int size[3];
} rle_info;

/* defines structure to hold rle table */
typedef struct {
	uint16_t info;
	uint8_t info_bits;
	uint32_t code;
	uint8_t code_bits;
} rle_table;

/*
	This function prints the run-length intermediate table
	Parameters:
		table: the table
		size: table size
	Return values:
		none
*/
void rle_print(rle **table, int *size);

/*
	This function applies the run-length encode process
	Parameters:
		handler: the file handler to output data
		vector: the input vector
		size: vector size
	Return values:
		RLE_OK: encode ok
		RLE_WERR: write error
		RLE_HERR: huffman error
*/
int rle_encode(FILE *handler, pixel_int32 **vector, int size);

/*
	This function applies the run-length decode process
	Parameters:
		handler: the file handler from where data will be read
		vector: the output vector
		size: vector size
	Return values:
		RLE_OK: decode ok
		RLE_RERR: read error
		RLE_FERR: file format error
*/
int rle_decode(FILE *handler, pixel_int32 **vector, int size);

/*
	This function frees the memory used by rle table
	Parameters:
		table: the rle table
		size: the rle table size
	Return values:
		none
*/
void rle_clean(rle **table, int size);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void rle_error(int code);

#endif
