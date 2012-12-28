/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __BINARY_H__
#define __BINARY_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* 8 bits conversion from reduced representation */
int8_t binary_expand8(uint8_t value, uint8_t size);
/* 8 bits conversion to reduced representation */
uint8_t binary_reduce8(int8_t value);
/* 8 bits size (bit count) for reduced representation */
int binary_size8(int8_t value);
/* 8 bits binary representation printer */
char *binary_print8(uint8_t value);
/* 8 bits binary mask creation */
uint8_t binary_mask8(uint8_t size);
/* 8 bits binary extraction */
uint8_t binary_extract8(uint8_t value, uint8_t offset, uint8_t size);

/* 32 bits conversion from reduced representation */
int32_t binary_expand32(uint32_t value, uint32_t size);
/* 32 bits conversion to reduced representation */
uint32_t binary_reduce32(int32_t value);
/* 32 bits size (bit count) for reduced representation */
int binary_size32(int32_t value);
/* 32 bits binary representation printer */
char *binary_print32(uint32_t value);
/* 32 bits binary mask creation */
uint32_t binary_mask32(uint32_t size);
/* 32 bits binary extraction */
uint32_t binary_extract32(uint32_t value, uint8_t offset, uint8_t size);

#endif
