#include <stdlib.h>

#include "heap.h"

static void swap(MaxHeap *heap, int first, int second) {
    Song *temporary = heap->songs[first];
    heap->songs[first] = heap->songs[second];
    heap->songs[second] = temporary;
    heap->songs[first]->heap_index = first;
    heap->songs[second]->heap_index = second;
}

static int grow(MaxHeap *heap) {
    int new_capacity = heap->capacity * 2;
    Song **resized = realloc(heap->songs, (size_t)new_capacity * sizeof(*resized));
    if (resized == NULL) {
        return 0;
    }
    heap->songs = resized;
    heap->capacity = new_capacity;
    return 1;
}

static void sift_up(MaxHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->songs[parent]->play_count >= heap->songs[index]->play_count) {
            break;
        }
        swap(heap, parent, index);
        index = parent;
    }
}

static void sift_down(MaxHeap *heap, int index) {
    for (;;) {
        int left = 2 * index + 1;
        int right = left + 1;
        int largest = index;

        if (left < heap->count &&
            heap->songs[left]->play_count > heap->songs[largest]->play_count) {
            largest = left;
        }
        if (right < heap->count &&
            heap->songs[right]->play_count > heap->songs[largest]->play_count) {
            largest = right;
        }
        if (largest == index) {
            return;
        }
        swap(heap, index, largest);
        index = largest;
    }
}

MaxHeap *heap_create(int initial_capacity) {
    if (initial_capacity < 1) {
        initial_capacity = 8;
    }

    MaxHeap *heap = malloc(sizeof(*heap));
    if (heap == NULL) {
        return NULL;
    }

    heap->songs = malloc((size_t)initial_capacity * sizeof(*heap->songs));
    if (heap->songs == NULL) {
        free(heap);
        return NULL;
    }
    heap->count = 0;
    heap->capacity = initial_capacity;
    return heap;
}

int heap_record_play(MaxHeap *heap, Song *song) {
    if (heap == NULL || song == NULL) {
        return 0;
    }

    song->play_count++;
    if (song->heap_index >= 0) {
        sift_up(heap, song->heap_index);
        return 1;
    }

    if (heap->count == heap->capacity && !grow(heap)) {
        song->play_count--;
        return 0;
    }

    int index = heap->count++;
    heap->songs[index] = song;
    song->heap_index = index;
    sift_up(heap, index);
    return 1;
}

const Song *heap_peek(const MaxHeap *heap) {
    return heap == NULL || heap->count == 0 ? NULL : heap->songs[0];
}

Song *heap_extract_max(MaxHeap *heap) {
    if (heap == NULL || heap->count == 0) {
        return NULL;
    }

    Song *maximum = heap->songs[0];
    maximum->heap_index = -1;
    heap->count--;
    if (heap->count > 0) {
        heap->songs[0] = heap->songs[heap->count];
        heap->songs[0]->heap_index = 0;
        sift_down(heap, 0);
    }
    return maximum;
}

void heap_print(const MaxHeap *heap) {
    if (heap == NULL || heap->count == 0) {
        puts("No songs have been played yet.");
        return;
    }

    puts("Popularity heap:");
    for (int index = 0; index < heap->count; index++) {
        printf("%d. ", index + 1);
        song_print(heap->songs[index], stdout);
        printf(" | plays: %d\n", heap->songs[index]->play_count);
    }
}

void heap_destroy(MaxHeap *heap) {
    if (heap == NULL) {
        return;
    }
    for (int index = 0; index < heap->count; index++) {
        heap->songs[index]->heap_index = -1;
    }
    free(heap->songs);
    free(heap);
}
