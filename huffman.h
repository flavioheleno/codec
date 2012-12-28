/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "list.h"
#include "tree.h"
#include "binary.h"

/* defines structure to hold huffman data */
typedef struct {
	int value;
	int code;
	int bits;
} huffman;

/*
	This function adds an value and its frequency to a list
	Parameters:
		head: list head pointer
		value
		freq: frequency of value
	Return values:
		none
*/
void huff_add(list **head, int value, int freq);

/*
	This function generates huffman code based on a list
	Parameters:
		head: list head pointer
		table: huffman table
	Return values:
		Table size
*/
int huff_code(list *head, huffman **table);

/*
	This function prints the huffman table
	Parameters:
		table: huffman table
		size: table size
	Return values:
		none
*/
void huff_print(huffman *table, int size);

/*
	This function cleans the huffman table
	Parameters:
		table: huffman table
		size: table size
	Return values:
		none
*/
void huff_clean(huffman **table, int size);

/*
	This function checks if a given code is a valid huffman code
	Parameters:
		table: huffman table
		size: table size
		code: the code to be checked
		bits: number of bits of the code
		value: stores the value represented by the huffman code
	Return values:
		1 if code is found
		0 case else
*/
int huff_check(huffman *table, int size, uint32_t code, int bits, int *value);

/*
	This function searchs for a given's value code
	Parameters:
		table: huffman table
		size: table size
		value: value to be searched
		code: stores the code (if found)
	Return values:
		number of bits of the code if value is found
		-1 if failed
*/
int huff_search(huffman *table, int size, int value, uint32_t *code);

#endif
