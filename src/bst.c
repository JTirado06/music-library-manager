#include <stdlib.h>
#include <string.h>

#include "bst.h"

static BSTNode *node_create(const Song *song) {
    BSTNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->song = song_clone(song);
    if (node->song == NULL) {
        free(node);
        return NULL;
    }

    node->left = NULL;
    node->right = NULL;
    return node;
}

int bst_insert(BSTNode **root, const Song *song) {
    if (root == NULL || song == NULL) {
        return 0;
    }

    if (*root == NULL) {
        *root = node_create(song);
        return *root != NULL;
    }

    int comparison = strcmp(song->title, (*root)->song->title);
    if (comparison < 0) {
        return bst_insert(&(*root)->left, song);
    }
    if (comparison > 0) {
        return bst_insert(&(*root)->right, song);
    }
    return 1;
}

const Song *bst_search(const BSTNode *root, const char *title) {
    if (root == NULL || title == NULL) {
        return NULL;
    }

    int comparison = strcmp(title, root->song->title);
    if (comparison == 0) {
        return root->song;
    }
    return comparison < 0 ? bst_search(root->left, title)
                          : bst_search(root->right, title);
}

void bst_print_inorder(const BSTNode *root) {
    if (root == NULL) {
        return;
    }
    bst_print_inorder(root->left);
    song_print(root->song, stdout);
    putchar('\n');
    bst_print_inorder(root->right);
}

int bst_height(const BSTNode *root) {
    if (root == NULL) {
        return -1;
    }
    int left = bst_height(root->left);
    int right = bst_height(root->right);
    return 1 + (left > right ? left : right);
}

void bst_destroy(BSTNode *root) {
    if (root == NULL) {
        return;
    }
    bst_destroy(root->left);
    bst_destroy(root->right);
    song_destroy(root->song);
    free(root);
}
