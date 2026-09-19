# Music Library Manager

A command-line music catalog and playlist manager written in C. The application combines linked data structures, balanced search trees, and a max heap to organize songs and track listening activity.

## Features

- Loads song records from a text file
- Stores the initial catalog in a binary search tree
- Transfers the catalog into an AVL tree for balanced title lookup
- Searches songs by exact title
- Maintains a linked-list playlist with insertion and removal
- Tracks recently played songs using a LIFO stack
- Uses a max heap to track the most frequently played songs
- Manages dynamically allocated strings, nodes, songs, and containers
- Reports the original BST and balanced AVL heights at startup

## Data Structures

| Structure | Purpose |
| --- | --- |
| Binary search tree | Imports and alphabetizes the initial catalog |
| AVL tree | Provides balanced title-based search |
| Singly linked list | Stores the user's playlist |
| Stack | Stores recently played songs in reverse chronological order |
| Max heap | Keeps the most-played song at the root |

## Project Structure

```text
music-library-manager/
├── data/
│   └── songs.txt
├── include/
│   ├── avl.h
│   ├── bst.h
│   ├── heap.h
│   ├── playlist.h
│   └── song.h
├── src/
│   ├── avl.c
│   ├── bst.c
│   ├── heap.c
│   ├── main.c
│   ├── playlist.c
│   └── song.c
├── Makefile
└── README.md
```

## Build and Run

Requirements:

- GCC or another C11-compatible compiler
- `make`

```bash
make
./music-library
```

Without `make`, compile directly with:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o music-library
./music-library
```

To load a different catalog:

```bash
./music-library path/to/songs.txt
```

Each catalog line uses this format:

```text
title,artist,duration_in_seconds
```

## Background

This project evolved from a three-stage data-structures course project. Earlier stages covered dynamic song storage, a linked-list playlist, and a recently played stack. A later stage supplied a driver and interfaces for a catalog application; I implemented the BST, AVL tree, and max-heap algorithms. I subsequently rewrote the supporting application, public interfaces, file loading, playlist module, and interactive menu to create this standalone portfolio version.

## Potential Improvements

- Case-insensitive and partial-title searching
- Saving playlists and play counts between sessions
- Multiple named playlists
- Unit tests for each data structure
- A graphical or web-based interface
