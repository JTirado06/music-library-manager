#ifndef BST_H
#define BST_H

#include "song.h"

typedef struct BSTNode {
    Song *song;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

int bst_insert(BSTNode **root, const Song *song);
const Song *bst_search(const BSTNode *root, const char *title);
void bst_print_inorder(const BSTNode *root);
int bst_height(const BSTNode *root);
void bst_destroy(BSTNode *root);

#endif
