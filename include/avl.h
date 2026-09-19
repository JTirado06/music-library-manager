#ifndef AVL_H
#define AVL_H

#include "song.h"

typedef struct AVLNode {
    Song *song;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

int avl_insert(AVLNode **root, const Song *song);
Song *avl_search(AVLNode *root, const char *title);
void avl_print_inorder(const AVLNode *root);
int avl_height(const AVLNode *root);
void avl_destroy(AVLNode *root);

#endif
