/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "list.h"

/* defines struct to hold sort data */
typedef struct sort {
	node *tree;
	float freq;
} sort;

/*
	This function compares two values
	Parameters:
		a: first value
		b: second value
	Return values:
		-1: if b is bigger than a
		0: if a and b are equal
		1: if a is bigger than b
*/
int sort_cmp(const void *a, const void *b) {
	if ((*(sort *)a).freq == (*(sort *)b).freq)
		return 0;
	return ((*(sort *)a).freq < (*(sort *)b).freq ? -1 : 1);
}

void list_clean(list **head) {
	list *p, *aux;
	p = *head;
	while (p) {
		aux = p;
		p = p->next;
		free(aux);
	}
	*head = NULL;
}

void list_add(list **head, node *data) {
	list *new = (list *)malloc(sizeof(list));
	list *p;
	new->data = data;
	new->next = NULL;
	if (*head == NULL)
		*head = new;
	else {
		p = *head;
		while (p->next)
			p = p->next;
		p->next = new;
	}
}

void list_del(list **head, int pos) {
	int i;
	list *p, *aux = NULL;
	if (*head != NULL) {
		p = *head;
		i = 0;
		while ((p->next) && (i < pos)) {
			aux = p;
			p = p->next;
			i++;
		}
		if (i == pos) {
			aux->next = p->next;
			free(p);
		}
	}
}

node *list_pop(list **head) {
	list *p;
	node *data = NULL;
	if (*head == NULL)
		return NULL;
	p = *head;
	*head = p->next;
	data = p->data;
	free(p);
	return data;
}

void list_push(list **head, node *data) {
	list *new = (list *)malloc(sizeof(list));
	new->data = data;
	new->next = *head;
	*head = new;
}

void list_print(list *head) {
	list *p;
	int i;
	if (head == NULL)
		printf("empty list\n");
	else {
		p = head;
		i = 0;
		while (p) {
			printf("value = %d (%d)\n", p->data->data.value, p->data->data.freq);
			p = p->next;
			i++;
		}
		printf("printed %d items\n", i);
	}
}

int list_size(list *head) {
	list *p = head;
	int i = 0;
	while (p) {
		p = p->next;
		i++;
	}
	return i;
}

void list_sort(list **head) {
	list *p = *head;
	int size = list_size(*head);
	sort *aux = (sort *)malloc(sizeof(sort) * size);
	int i;
	for (i = 0; i < size; i++) {
		aux[i].tree = p->data;
		aux[i].freq = p->data->data.freq;
		p = p->next;
	}
	list_clean(head);
	qsort(aux, size, sizeof(sort), sort_cmp);
	for (i = 0; i < size; i++)
		list_add(head, aux[i].tree);
}

void list_clone(list **target, list *source) {
	list *p;
	p = source;
	while (p) {
		list_add(target, p->data);
		p = p->next;
	}
}
