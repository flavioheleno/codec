/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __CODEC_H__
#define __CODEC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "file.h"

/* defines codec id */
#define CODEC_ID 0x21424846

/* defines return values */
#define COD_OK 0x00
#define COD_WERR 0x01
#define COD_RERR 0x02
#define COD_INV 0x03


/*
	This function writes the codec header to file
	Parameters:
		handler: a file pointer
		width: image width
		height: image height
	Return values:
		COD_OK: write ok
		COD_WERR: write error
*/
int codec_write_header(FILE *handler, uint32_t width, uint32_t height);

/*
	This function reads the codec header from file
	Parameters:
		handler: a file pointer
		width: variable that will store image width
		height: variable that will store image height
	Return values:
		COD_OK: read ok
		COD_RERR: read error
		COD_INV: invalid file
*/
int codec_read_header(FILE *handler, uint32_t *width, uint32_t *height);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void codec_error(int code);

#endif
