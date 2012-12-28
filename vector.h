/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdint.h>

#include "pixel.h"
#include "dct.h"

/* defines return values */
#define VEC_OK 0x00
#define VEC_MERR 0x01

/* defines structure to hold vector data */
typedef struct {
	int size;
	pixel_int32 **data;
	pixel_int32 *dc;
} vector_data;

/*
	This function prints vector data
	Parameters:
		data: vector data to be printed
	Return values:
		none
*/
void vector_print(vector_data data);

/*
	This function initializes vector struct
	Parameters:
		target: vector struct to be initialized
		width: base image width
		height: base image height
	Return values:
		VEC_OK: initialization ok
		VEC_MERR: malloc error
*/
int vector_init(vector_data *target, uint32_t width, uint32_t height);

/*
	This function converts a block to a vector
	Parameters:
		block: block data to be converted
		target: variable to store converted vector data
	Return values:
		VEC_OK: conversion ok
*/
int vector_from_block(dct_data block, vector_data *target);

/*
	This function converts a vector to a block
	Parameters:
		source: vector data to be converted
		block: variable to store converted block data
	Return values:
		VEC_OK: conversion ok
*/
int vector_to_block(vector_data source, dct_data *block);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void vector_error(int code);

#endif
