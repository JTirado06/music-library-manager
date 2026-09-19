#ifndef SONG_H
#define SONG_H

#include <stdio.h>

typedef struct {
    char *title;
    char *artist;
    int duration_seconds;
    int play_count;
    int heap_index;
} Song;

Song *song_create(const char *title, const char *artist, int duration_seconds);
Song *song_clone(const Song *source);
void song_print(const Song *song, FILE *stream);
void song_destroy(Song *song);

#endif
