#ifndef HEAP_H
#define HEAP_H

#include "song.h"

typedef struct {
    Song **songs;
    int count;
    int capacity;
} MaxHeap;

MaxHeap *heap_create(int initial_capacity);
int heap_record_play(MaxHeap *heap, Song *song);
const Song *heap_peek(const MaxHeap *heap);
Song *heap_extract_max(MaxHeap *heap);
void heap_print(const MaxHeap *heap);
void heap_destroy(MaxHeap *heap);

#endif
