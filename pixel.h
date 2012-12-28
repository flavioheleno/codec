/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <stdint.h>

/* defines 8bit unsigned pixel data structure */
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel_uint8;

/* defines 8bit signed pixel data structure */
typedef struct {
	int8_t r;
	int8_t g;
	int8_t b;
} pixel_int8;

/* defines 16bit unsigned pixel data structure */
typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
} pixel_uint16;

/* defines 16bit signed pixel data structure */
typedef struct {
	int16_t r;
	int16_t g;
	int16_t b;
} pixel_int16;

/* defines 32bit unsigned pixel data structure */
typedef struct {
	uint32_t r;
	uint32_t g;
	uint32_t b;
} pixel_uint32;

/* defines 32bit signed pixel data structure */
typedef struct {
	int32_t r;
	int32_t g;
	int32_t b;
} pixel_int32;

#endif
