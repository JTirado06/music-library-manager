#include <stdlib.h>
#include <string.h>

#include "avl.h"

static int max_int(int first, int second) {
    return first > second ? first : second;
}

static int node_height(const AVLNode *node) {
    return node == NULL ? -1 : node->height;
}

static void update_height(AVLNode *node) {
    node->height = 1 + max_int(node_height(node->left), node_height(node->right));
}

static AVLNode *node_create(const Song *song) {
    AVLNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->song = song_clone(song);
    if (node->song == NULL) {
        free(node);
        return NULL;
    }

    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static AVLNode *rotate_left(AVLNode *root) {
    AVLNode *new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    update_height(root);
    update_height(new_root);
    return new_root;
}

static AVLNode *rotate_right(AVLNode *root) {
    AVLNode *new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    update_height(root);
    update_height(new_root);
    return new_root;
}

static AVLNode *insert_recursive(AVLNode *root, const Song *song, int *success) {
    if (root == NULL) {
        AVLNode *created = node_create(song);
        *success = created != NULL;
        return created;
    }

    int comparison = strcmp(song->title, root->song->title);
    if (comparison < 0) {
        AVLNode *updated = insert_recursive(root->left, song, success);
        if (*success) {
            root->left = updated;
        }
    } else if (comparison > 0) {
        AVLNode *updated = insert_recursive(root->right, song, success);
        if (*success) {
            root->right = updated;
        }
    } else {
        *success = 1;
        return root;
    }

    if (!*success) {
        return root;
    }

    update_height(root);
    int balance = node_height(root->left) - node_height(root->right);

    if (balance > 1) {
        if (strcmp(song->title, root->left->song->title) > 0) {
            root->left = rotate_left(root->left);
        }
        return rotate_right(root);
    }

    if (balance < -1) {
        if (strcmp(song->title, root->right->song->title) < 0) {
            root->right = rotate_right(root->right);
        }
        return rotate_left(root);
    }

    return root;
}

int avl_insert(AVLNode **root, const Song *song) {
    if (root == NULL || song == NULL) {
        return 0;
    }
    int success = 0;
    *root = insert_recursive(*root, song, &success);
    return success;
}

Song *avl_search(AVLNode *root, const char *title) {
    if (root == NULL || title == NULL) {
        return NULL;
    }
    int comparison = strcmp(title, root->song->title);
    if (comparison == 0) {
        return root->song;
    }
    return comparison < 0 ? avl_search(root->left, title)
                          : avl_search(root->right, title);
}

void avl_print_inorder(const AVLNode *root) {
    if (root == NULL) {
        return;
    }
    avl_print_inorder(root->left);
    song_print(root->song, stdout);
    printf(" | plays: %d\n", root->song->play_count);
    avl_print_inorder(root->right);
}

int avl_height(const AVLNode *root) {
    return node_height(root);
}

void avl_destroy(AVLNode *root) {
    if (root == NULL) {
        return;
    }
    avl_destroy(root->left);
    avl_destroy(root->right);
    song_destroy(root->song);
    free(root);
}
