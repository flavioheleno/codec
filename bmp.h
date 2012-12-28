/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pixel.h"

/* struct for bmp file header and bmp info header */
typedef struct {
	uint32_t fsize;
	uint32_t reserved;
	uint32_t offset;
	uint32_t hsize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t dsize;
	uint32_t hres;
	uint32_t vres;
	uint32_t colors;
	uint32_t important;
} bmp_header;

/* struct for bitmap file */
typedef struct {
	bmp_header header;
	pixel_uint8 **data;
} bmp_file;

/* defines return values */
#define BMP_OK 0x00
#define BMP_FERR 0x01
#define BMP_RERR 0x02
#define BMP_IFF 0x03
#define BMP_SERR 0x04
#define BMP_MERR 0x05
#define BMP_WERR 0x06

/*
	This function prints bmp header info
	Parameters:
		header: variable that holds bmp header
	Return values:
		none
*/
void bmp_print_header(bmp_header header);

/*
	This function prints pixel values stored on data array
	Parameters:
		data: variable that holds pixels values
		width: base image width
		height: base image height
	Return values:
		none
*/
void bmp_print_data(bmp_file bmp);

/*
	This function frees the memory used by data array
	Parameters:
		data: variable that holds pixel values
		size: image width
	Return values:
		none
*/
void bmp_free_data(bmp_file *bmp);

/*
	This function loads bmp header and data of the given file
	Parameters:
		filename: full path to the bmp file to be loaded
		header: variable that will hold bmp header
		data: variable that will hold pixel values
	Return values:
		BMP_OK: load ok
		BMP_FERR: file error
		BMP_RERR: read error
		BMP_IBF: invalid bmp file
		BMP_SERR: seek error
		BMP_MERR: malloc error
*/
int bmp_load(const char *filename, bmp_file *bmp);

/*
	This function saves bmp header and data into the given file, respecting BMP File Format
	Parameters:
		filename: full path to the bmp file to be saved
		header: variable that holds bmp header
		data: variable that holds pixel values
	Return values:
		BMP_OK: save ok
		BMP_FERR: file error
		BMP_WERR: write error
*/
int bmp_save(const char *filename, bmp_file bmp);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void bmp_error(int code);

#endif
