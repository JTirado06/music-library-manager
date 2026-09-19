#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "bst.h"
#include "heap.h"
#include "playlist.h"
#include "song.h"

#define INPUT_SIZE 256

static void trim_newline(char *text) {
    text[strcspn(text, "\r\n")] = '\0';
}

static int read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    trim_newline(buffer);
    return 1;
}

static int read_integer(const char *prompt, int *value) {
    char buffer[INPUT_SIZE];
    char trailing;
    if (!read_line(prompt, buffer, sizeof(buffer))) {
        return 0;
    }
    return sscanf(buffer, " %d %c", value, &trailing) == 1;
}

static int parse_song_line(char *line, char **title, char **artist, int *duration) {
    char *first = strchr(line, ',');
    if (first == NULL) {
        return 0;
    }
    char *second = strchr(first + 1, ',');
    if (second == NULL) {
        return 0;
    }

    *first = '\0';
    *second = '\0';
    *title = line;
    *artist = first + 1;
    while (**artist == ' ') {
        (*artist)++;
    }

    char trailing;
    return sscanf(second + 1, " %d %c", duration, &trailing) == 1;
}

static int load_catalog(const char *filename, BSTNode **catalog, int *song_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char line[INPUT_SIZE];
    while (fgets(line, sizeof(line), file) != NULL) {
        trim_newline(line);
        char *title;
        char *artist;
        int duration;
        if (!parse_song_line(line, &title, &artist, &duration)) {
            continue;
        }

        Song *song = song_create(title, artist, duration);
        if (song == NULL || !bst_insert(catalog, song)) {
            song_destroy(song);
            fclose(file);
            return 0;
        }
        song_destroy(song);
        (*song_count)++;
    }

    fclose(file);
    return 1;
}

static int transfer_to_avl(const BSTNode *source, AVLNode **destination) {
    if (source == NULL) {
        return 1;
    }
    return avl_insert(destination, source->song) &&
           transfer_to_avl(source->left, destination) &&
           transfer_to_avl(source->right, destination);
}

static Song *prompt_for_song(AVLNode *catalog) {
    char title[INPUT_SIZE];
    if (!read_line("Enter the exact song title: ", title, sizeof(title))) {
        return NULL;
    }
    Song *song = avl_search(catalog, title);
    if (song == NULL) {
        puts("Song not found.");
    }
    return song;
}

static void print_menu(void) {
    puts("\nMusic Library Manager");
    puts("1. Display catalog");
    puts("2. Search for a song");
    puts("3. Add a song to playlist");
    puts("4. Remove a song from playlist");
    puts("5. Display playlist");
    puts("6. Play a song");
    puts("7. Display recently played");
    puts("8. Display popularity heap");
    puts("9. Exit");
}

int main(int argc, char **argv) {
    const char *filename = argc > 1 ? argv[1] : "data/songs.txt";
    BSTNode *bst_catalog = NULL;
    AVLNode *catalog = NULL;
    int song_count = 0;

    if (!load_catalog(filename, &bst_catalog, &song_count)) {
        fprintf(stderr, "Could not load catalog from %s.\n", filename);
        bst_destroy(bst_catalog);
        return EXIT_FAILURE;
    }

    int original_height = bst_height(bst_catalog);
    if (!transfer_to_avl(bst_catalog, &catalog)) {
        fputs("Could not build the balanced catalog.\n", stderr);
        bst_destroy(bst_catalog);
        avl_destroy(catalog);
        return EXIT_FAILURE;
    }
    bst_destroy(bst_catalog);

    Playlist *playlist = playlist_create("My Playlist");
    PlayHistory *history = history_create();
    MaxHeap *popularity = heap_create(song_count);
    if (playlist == NULL || history == NULL || popularity == NULL) {
        fputs("Could not initialize application data.\n", stderr);
        playlist_destroy(playlist);
        history_destroy(history);
        heap_destroy(popularity);
        avl_destroy(catalog);
        return EXIT_FAILURE;
    }

    printf("Loaded %d songs. BST height: %d, AVL height: %d.\n",
           song_count, original_height, avl_height(catalog));

    int running = 1;
    while (running) {
        print_menu();
        int choice;
        if (!read_integer("Choose an option: ", &choice)) {
            puts("Please enter a number from 1 to 9.");
            continue;
        }

        Song *song;
        int position;
        switch (choice) {
            case 1:
                avl_print_inorder(catalog);
                break;
            case 2:
                song = prompt_for_song(catalog);
                if (song != NULL) {
                    song_print(song, stdout);
                    printf(" | plays: %d\n", song->play_count);
                }
                break;
            case 3:
                song = prompt_for_song(catalog);
                if (song != NULL) {
                    puts(playlist_add(playlist, song)
                             ? "Song added to playlist."
                             : "Could not add song.");
                }
                break;
            case 4:
                playlist_print(playlist);
                if (read_integer("Position to remove: ", &position)) {
                    puts(playlist_remove(playlist, position)
                             ? "Song removed."
                             : "Invalid playlist position.");
                }
                break;
            case 5:
                playlist_print(playlist);
                break;
            case 6:
                song = prompt_for_song(catalog);
                if (song != NULL && history_push(history, song) &&
                    heap_record_play(popularity, song)) {
                    printf("Now playing: ");
                    song_print(song, stdout);
                    putchar('\n');
                } else if (song != NULL) {
                    puts("Could not record playback.");
                }
                break;
            case 7:
                history_print(history);
                break;
            case 8:
                heap_print(popularity);
                if (heap_peek(popularity) != NULL) {
                    printf("Most popular: ");
                    song_print(heap_peek(popularity), stdout);
                    putchar('\n');
                }
                break;
            case 9:
                running = 0;
                break;
            default:
                puts("Please enter a number from 1 to 9.");
        }
    }

    playlist_destroy(playlist);
    history_destroy(history);
    heap_destroy(popularity);
    avl_destroy(catalog);
    puts("Goodbye.");
    return EXIT_SUCCESS;
}
