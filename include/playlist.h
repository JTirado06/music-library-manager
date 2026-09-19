#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"

typedef struct SongNode {
    Song *song;
    struct SongNode *next;
} SongNode;

typedef struct {
    char *name;
    SongNode *head;
    SongNode *tail;
    int count;
} Playlist;

typedef struct {
    SongNode *top;
    int count;
} PlayHistory;

Playlist *playlist_create(const char *name);
int playlist_add(Playlist *playlist, const Song *song);
int playlist_remove(Playlist *playlist, int position);
const Song *playlist_get(const Playlist *playlist, int position);
void playlist_print(const Playlist *playlist);
void playlist_destroy(Playlist *playlist);

PlayHistory *history_create(void);
int history_push(PlayHistory *history, const Song *song);
void history_print(const PlayHistory *history);
void history_destroy(PlayHistory *history);

#endif
