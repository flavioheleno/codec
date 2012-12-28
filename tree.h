/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint32_t freq;
	uint32_t value;
} node_data;

typedef struct tn {
	node_data data;
	struct tn *left;
	struct tn *right;
} node;

/*
	This function frees the memory used by tree
	Parameters:
		root: tree root
	Return values:
		none
*/
void tree_clean(node **root);

/*
	This function creates a new node
	Parameters:
		data: node data
		left: left node
		right: right node
	Return values:
		the created node pointer if success
		NULL if failed
*/
node *tree_create_node(node_data data, node *left, node *right);

/*
	This function prints the tree structure
	Parameters:
		root: tree root
	Return values:
		none
*/
void tree_print(node *root);


#endif
