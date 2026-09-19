#include <stdlib.h>
#include <string.h>

#include "playlist.h"

static char *duplicate_string(const char *source) {
    size_t length = strlen(source) + 1;
    char *copy = malloc(length);
    if (copy != NULL) {
        memcpy(copy, source, length);
    }
    return copy;
}

static SongNode *node_create(const Song *song) {
    SongNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->song = song_clone(song);
    if (node->song == NULL) {
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

Playlist *playlist_create(const char *name) {
    if (name == NULL) {
        return NULL;
    }

    Playlist *playlist = calloc(1, sizeof(*playlist));
    if (playlist == NULL) {
        return NULL;
    }
    playlist->name = duplicate_string(name);
    if (playlist->name == NULL) {
        free(playlist);
        return NULL;
    }
    return playlist;
}

int playlist_add(Playlist *playlist, const Song *song) {
    if (playlist == NULL || song == NULL) {
        return 0;
    }
    SongNode *node = node_create(song);
    if (node == NULL) {
        return 0;
    }

    if (playlist->tail == NULL) {
        playlist->head = node;
    } else {
        playlist->tail->next = node;
    }
    playlist->tail = node;
    playlist->count++;
    return 1;
}

int playlist_remove(Playlist *playlist, int position) {
    if (playlist == NULL || position < 1 || position > playlist->count) {
        return 0;
    }

    SongNode *removed;
    if (position == 1) {
        removed = playlist->head;
        playlist->head = removed->next;
    } else {
        SongNode *previous = playlist->head;
        for (int index = 2; index < position; index++) {
            previous = previous->next;
        }
        removed = previous->next;
        previous->next = removed->next;
        if (removed == playlist->tail) {
            playlist->tail = previous;
        }
    }

    if (playlist->head == NULL) {
        playlist->tail = NULL;
    }
    song_destroy(removed->song);
    free(removed);
    playlist->count--;
    return 1;
}

const Song *playlist_get(const Playlist *playlist, int position) {
    if (playlist == NULL || position < 1 || position > playlist->count) {
        return NULL;
    }
    SongNode *current = playlist->head;
    for (int index = 1; index < position; index++) {
        current = current->next;
    }
    return current->song;
}

void playlist_print(const Playlist *playlist) {
    if (playlist == NULL) {
        return;
    }
    printf("Playlist: %s\n", playlist->name);
    if (playlist->head == NULL) {
        puts("  Empty");
        return;
    }

    int position = 1;
    for (SongNode *current = playlist->head; current != NULL; current = current->next) {
        printf("%d. ", position++);
        song_print(current->song, stdout);
        putchar('\n');
    }
}

void playlist_destroy(Playlist *playlist) {
    if (playlist == NULL) {
        return;
    }
    SongNode *current = playlist->head;
    while (current != NULL) {
        SongNode *next = current->next;
        song_destroy(current->song);
        free(current);
        current = next;
    }
    free(playlist->name);
    free(playlist);
}

PlayHistory *history_create(void) {
    return calloc(1, sizeof(PlayHistory));
}

int history_push(PlayHistory *history, const Song *song) {
    if (history == NULL || song == NULL) {
        return 0;
    }
    SongNode *node = node_create(song);
    if (node == NULL) {
        return 0;
    }
    node->next = history->top;
    history->top = node;
    history->count++;
    return 1;
}

void history_print(const PlayHistory *history) {
    if (history == NULL || history->top == NULL) {
        puts("Recently played: Empty");
        return;
    }

    puts("Recently played (newest first):");
    int position = 1;
    for (SongNode *current = history->top; current != NULL; current = current->next) {
        printf("%d. ", position++);
        song_print(current->song, stdout);
        putchar('\n');
    }
}

void history_destroy(PlayHistory *history) {
    if (history == NULL) {
        return;
    }
    SongNode *current = history->top;
    while (current != NULL) {
        SongNode *next = current->next;
        song_destroy(current->song);
        free(current);
        current = next;
    }
    free(history);
}
