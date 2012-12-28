/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "huffman.h"

void huff_add(list **head, int value, int freq) {
	node_data item;
	node *data;
	item.value = value;
	item.freq = freq;
	data = tree_create_node(item, NULL, NULL);
	list_add(head, data);
}

/*
	This function navigates on huffman tree, generating the codes
	Parameters:
		root: pointer to tree root
		code: current code
		bits: number of bits used to generate huffman code
		table: huffman table
		table_pos: current table position
	Return values:
		none
*/
void navigate(node *root, int code, int bits, huffman *table, int *table_pos) {
	if (root != NULL) {
		if ((root->left == NULL) && (root->right == NULL)) {
			table[*table_pos].value = root->data.value;
			table[*table_pos].code = code;
			table[*table_pos].bits = (bits + 1);
			(*table_pos)++;
		} else {
			code <<= 1;
			bits++;
			navigate(root->left, code, bits, table, table_pos);
			code |= 1;
			navigate(root->right, code, bits, table, table_pos);
		}
	}
}

int huff_code(list *head, huffman **table) {
	list *huff = NULL;
	node *tmp, *left, *right;
	node_data item;
	huffman *tbl;
	int size = list_size(head), code, bits, pos;
	if (size == 1) {
		tbl = (huffman *)malloc(sizeof(huffman));
		if (tbl == NULL)
			return 0;
		tbl[0].value = head->data->data.value;
		tbl[0].code = 0;
		tbl[0].bits = 1;
	} else {
		list_clone(&huff, head);
		while (list_size(huff) > 1) {
			list_sort(&huff);
			left = list_pop(&huff);
			right = list_pop(&huff);
			item.freq = left->data.freq + right->data.freq;
			item.value = 0;
			tmp = tree_create_node(item, left, right);
			list_add(&huff, tmp);
		}
		tbl = (huffman *)malloc(sizeof(huffman) * size);
		if (tbl == NULL)
			return 0;
		tmp = list_pop(&huff);
		pos = 0;
		bits = 0;
		code = 0;
		navigate(tmp->left, code, bits, tbl, &pos);
		bits = 0;
		code = 1;
		navigate(tmp->right, code, bits, tbl, &pos);
		list_clean(&huff);
	}
	*table = tbl;
	return size;
}

void huff_print(huffman *table, int size) {
	int i;
	printf("printing huffman table\n");
	if (table == NULL)
		printf("empty table\n");
	else {
		printf("value\tcode\tbits\n");
		for (i = 0; i < size; i++)
			printf("%d\t%X\t%d\n", table[i].value, table[i].code, table[i].bits);
	}
}

void huff_clean(huffman **table, int size) {
	/*
	int i;
	for (i = 0; i < size; i++)
		free(table[i]);
	*/
	*table = NULL;
}

int huff_check(huffman *table, int size, uint32_t code, int bits, int *value) {
	int i = 0, f = 1;
	uint32_t m1, m2;
	while (f) {
		if (table[i].bits == bits) {
			m1 = binary_mask32(table[i].bits);
			m2 = binary_mask32(bits);
			if (((uint32_t)table[i].code & m1) == (code & m2)) {
				*value = table[i].value;
				return 1;
			}
		}
		i++;
		if (i == size)
			f = 0;
	}
	return 0;
}

int huff_search(huffman *table, int size, int value, uint32_t *code) {
	int f = 1, i = 0, ret = -1;
	while ((f) && (i < size)) {
		if (table[i].value == value) {
			ret = table[i].bits;
			*code = table[i].code;
			f = 0;
		}
		i++;
	}
	return ret;
}
