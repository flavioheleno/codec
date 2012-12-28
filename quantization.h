/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __QUANTIZATION_H__
#define __QUANTIZATION_H__

#include "dct.h"

/* defines return values */
#define QUA_OK 0x00

/*
	This function applies the quantization to the given data
	Parameters:
		dct: data to be processed
	Return values:
		QUA_OK; operation ok
*/
int quantization(dct_data *dct);

/*
	This function applies the inverse quantization to the given data
	Parameters:
		dct: data to be processed
	Return values:
		QUA_OK; operation ok
*/
int quantization_inverse(dct_data *dct);

/*
	This function prints error messages
	Parameters:
		code: error code
	Return values:
		none
*/
void quantization_error(int code);

#endif
