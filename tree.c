/*

	Authors
	-------
	Aline Abreu Santos		6552035
	Diogo Ferrari Meletto	5890201
	Flávio Heleno Batista	5890027

*/

#include "tree.h"

void tree_clean(node **root) {
	if (*root != NULL) {
		tree_clean(&(*root)->left);
		tree_clean(&(*root)->right);
		free(*root);
	}
}

node *tree_create_node(node_data data, node *left, node *right) {
	node *new = (node *)malloc(sizeof(node));
	new->data = data;
	new->left = left;
	new->right = right;
	return new;
}

void tree_print(node *root) {
	printf("value: %d\n", root->data.value);
	if (root->left) {
		printf("left\n");
		tree_print(root->left);
	} else
		printf("left node is empty\n");
	if (root->right) {
		printf("right\n");
		tree_print(root->right);
	} else
		printf("right node is empty\n");
}
