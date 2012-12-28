/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __DCT_H__
#define __DCT_H__

#include <math.h>

#include "pixel.h"
#include "block.h"

/* defines struct of basic dct block */
typedef struct {
	pixel_int32 data[8][8];
} dct_block;

/* defines struct to hold dct data */
typedef struct {
	uint32_t width;
	uint32_t height;
	dct_block **block;
} dct_data;

/* defines return values */
#define DCT_OK 0x00
#define DCT_MERR 0x01

/*
	This function initializes dct struct
	Parameters:
		target: dct struct
		width: base image width
		height: base image height
	Return values:
		DCT_OK: initialization ok
		DCT_MERR: malloc error
*/
int dct_init(dct_data *target, int width, int height);

/*
	This function applies the dct
	Parameters:
		source: image data
		target: result data
	Return values:
		DCT_OK: operation ok
		DCT_MERR: malloc error
*/
int dct(bmp_block source, dct_data *target);

/*
	This function applies the inverse dct
	Parameters:
		source: dct'ed data
		target: image data
	Return values:
		DCT_OK: operation ok
		DCT_MERR: malloc error
*/
int dct_inverse(dct_data source, bmp_block *target);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void dct_error(int code);

#endif
