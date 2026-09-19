#include <stdlib.h>
#include <string.h>

#include "song.h"

static char *duplicate_string(const char *source) {
    size_t length = strlen(source) + 1;
    char *copy = malloc(length);
    if (copy != NULL) {
        memcpy(copy, source, length);
    }
    return copy;
}

Song *song_create(const char *title, const char *artist, int duration_seconds) {
    if (title == NULL || artist == NULL || duration_seconds < 0) {
        return NULL;
    }

    Song *song = calloc(1, sizeof(*song));
    if (song == NULL) {
        return NULL;
    }

    song->title = duplicate_string(title);
    song->artist = duplicate_string(artist);
    if (song->title == NULL || song->artist == NULL) {
        song_destroy(song);
        return NULL;
    }

    song->duration_seconds = duration_seconds;
    song->heap_index = -1;
    return song;
}

Song *song_clone(const Song *source) {
    if (source == NULL) {
        return NULL;
    }

    Song *copy = song_create(source->title, source->artist, source->duration_seconds);
    if (copy != NULL) {
        copy->play_count = source->play_count;
    }
    return copy;
}

void song_print(const Song *song, FILE *stream) {
    if (song == NULL || stream == NULL) {
        return;
    }

    fprintf(stream, "%s - %s (%d:%02d)", song->title, song->artist,
            song->duration_seconds / 60, song->duration_seconds % 60);
}

void song_destroy(Song *song) {
    if (song == NULL) {
        return;
    }
    free(song->title);
    free(song->artist);
    free(song);
}
