/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

/* defines struct to hold list data (linked list) */
typedef struct list_item {
	node *data;
	struct list_item *next;
} list;

/*
	This function frees the memory used by the list
	Parameters:
		head: the list
	Return values:
		none
*/
void list_clean(list **head);

/*
	This function adds the element to the end of the list
	Parameters:
		head: the list
		data: the new element
	Return values:
		none
*/
void list_add(list **head, node *data);

/*
	This function deletes the element at pos position from the list
	Parameters:
		head: the list
		pos: the element position
	Return values:
		none
*/
void list_del(list **head, int pos);

/*
	This function returns the element at list's begin
	Parameters:
		head: the list
	Return values:
		the element at list begin
*/
node *list_pop(list **head);

/*
	This function adds a new element on list's begin
	Parameters:
		head: the list
		data: the element to be added
	Return values:
		none
*/
void list_push(list **head, node *data);

/*
	This function prints all elements in list
	Parameters:
		head: the list
	Return values:
		none
*/
void list_print(list *head);

/*
	This functions performs a quick sort in list elements
	Parameters:
		head: the list
	Return values:
		none;
*/
void list_sort(list **head);

/*
	This function retrieves the number of elements in a list
	Parameters:
		head: the list
	Return values:
		number of elements in head list
*/
int list_size(list *head);

/*
	This function creates a copy of the source list into the target list
	Parameters:
		target: target list
		source: source list
	Return values:
		none
*/
void list_clone(list **target, list *source);

#endif
