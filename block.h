/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <math.h>

#include "pixel.h"
#include "bmp.h"

/* struct for block (8x8 array) */
typedef struct {
	pixel_int8 data[8][8];
} data_block;

/* struct for image blocks */
typedef struct {
	uint32_t width;
	uint32_t height;
	data_block **block;
} bmp_block;

/* defines return values */
#define BLK_OK 0x00
#define BLK_MERR 0x01

/*
	This function initializes the memory used by block data
	Parameters:
		blocks: variable that holds block data
		width: original image width
		height: original image height
	Return values:
		BLK_OK: blocks initialized
		BLK_MERR: malloc error
*/
int block_init(bmp_block *blocks, int width, int height);

/*
	This function frees the memory used by block data
	Parameters:
		blocks: variable that holds block data
	Return values:
		none
*/
void block_free_data(bmp_block *blocks);

/*
	This function creates 8x8 block from bitmap data
	Parameters:
		blocks: variable that will hold block data
		bmp: variable that holds bitmap file data
	Return values:
		BLK_OK: blocks created
		BLK_MERR: malloc error
*/
int block_from_bmp(bmp_block *blocks, bmp_file bmp);

/*
	This function recreates bitmap imagem based on block data
	Parameters:
		blocks: variable that holds block data
		bmp: variable that will hold bitmap file data
	Return values:
		BLK_OK: image recreated
		BLK_MERR: malloc error
*/
int block_to_bmp(bmp_block blocks, bmp_file *bmp);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void block_error(int code);

#endif
