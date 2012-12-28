/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "block.h"
#include "binary.h"

/* defines return values */
#define FILE_OK 0x00
#define FILE_FERR 0x01
#define FILE_WERR 0x02
#define FILE_IFF 0x03
#define FILE_MERR 0x04
#define FILE_RERR 0x05
#define FILE_SERR 0x06

/*
	This function compresses block data and writes it to file
	Parameters:
		filename: full path to the file to be written
		blocks: block data
	Return values:
		FILE_OK: file written
		FILE_FERR: file error
		FILE_WERR: write error
*/
int file_write(const char *filename, bmp_block blocks);

/*
	This function reads compressed block data from file and decompress it
	Parameters:
		filename: full path to the file to be read
		blocks: block data
		table: bit size table
		width: block width (bitmap width / 8)
		height: block height (bitmap height / 8)
	Return values:
		FILE_OK: load ok
		FILE_FERR: file error
		FILE_RERR: read error
		FILE_IFF: invalid file format
		FILE_MERR: malloc error
		FILE_SERR: seek error
*/
int file_read(const char *filename, bmp_block *blocks, uint8_t **table, uint32_t width, uint32_t height);

/*
	This function opens a file on write mode
	Parameters:
		filename: full path to the file to be opened
	Return values:
		NULL: if open fails
		File pointer: if open is ok
*/
FILE *file_open_write(const char *filename);

/*
	This function opens a file on read mode
	Parameters:
		filename: full path to the file to be opened
	Return values:
		NULL: if open fails
		File pointer: if open is ok
*/
FILE *file_open_read(const char *filename);

/*
	This function closes file opened by file_open_* functions
	Parameters:
		handler: file pointer
	Return values:
		FILE_OK: closed
		FILE_FERR: file error
*/
int file_close(FILE *handler);

/*
	This function writes 1 bytes to file
	Parameters:
		handler: file pointer
		data: data to be written
	Return values:
		FILE_OK: write ok
		FILE_WERR: write error
		FILE_FERR: file error
*/
int file_write8(FILE *handler, uint8_t data);

/*
	This function writes 2 bytes to file
	Parameters:
		handler: file pointer
		data: data to be written
	Return values:
		FILE_OK: write ok
		FILE_WERR: write error
		FILE_FERR: file error
*/
int file_write16(FILE *handler, uint16_t data);

/*
	This function writes 4 bytes to file
	Parameters:
		handler: file pointer
		data: data to be written
	Return values:
		FILE_OK: write ok
		FILE_WERR: write error
		FILE_FERR: file error
*/
int file_write32(FILE *handler, uint32_t data);

/*
	This function writes data from buffer into file
	Parameters:
		handler: file pointer
		data: buffer data
		size: number of vectors to be written
	Return values:
		FILE_OK: read ok
		FILE_WERR: write error
*/
int file_write_buffered(FILE *handler, uint32_t data, uint8_t bits, int flush);

/*
	This function reads 1 bytes from file
	Parameters:
		handler: file pointer
		data: data read
	Return values:
		FILE_OK: write ok
		FILE_RERR: read error
		FILE_FERR: file error
*/
int file_read8(FILE *handler, uint8_t *data);

/*
	This function reads 2 bytes from file
	Parameters:
		handler: file pointer
		data: data read
	Return values:
		FILE_OK: write ok
		FILE_RERR: read error
		FILE_FERR: file error
*/
int file_read16(FILE *handler, uint16_t *data);

/*
	This function reads 4 bytes from file
	Parameters:
		handler: file pointer
		data: data read
	Return values:
		FILE_OK: write ok
		FILE_RERR: read error
		FILE_FERR: file error
*/
int file_read32(FILE *handler, uint32_t *data);

/*
	This function creates a buffer and reads data from file into buffer
	Parameters:
		handler: file pointer
		data: array to store data
		size: number of vectors to be read
	Return values:
		FILE_OK: read ok
		FILE_MERR: malloc error
		FILE_RERR: read error
*/
int file_read_buffered(FILE *handler, uint32_t **data, uint16_t size);

#endif
