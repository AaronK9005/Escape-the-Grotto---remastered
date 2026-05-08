#include "world/chunk.h"

#include <stdio.h>
#include <stdlib.h>

chunk_t* chunk_load_from_file(chunk_t* dest, const char* path) {
    if (!dest) return NULL;

    FILE* f = fopen(path, "rb");
    if (!f) return NULL;

    if (fread(dest, sizeof(chunk_t), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    // TODO dynamic data

    fclose(f);
    return dest;
}

int chunk_save_to_file(chunk_t* chunk, const char* path) {
    if (!chunk) return EOF;

    FILE* f = fopen(path, "wb");
    if (!f) return EOF;

    if (fwrite(chunk, sizeof(chunk_t), 1, f) != 1) {
        fclose(f);
        return EOF;
    }

    // TODO dynamic data

    fclose(f);
    return 0;
}

// TODO add worlds in save system
int chunk_save(chunk_t* chunk, int floor_num) {
    char file_name[64];
    snprintf(file_name, sizeof(file_name),
        "saved/floor_%d/chunk_(%d;%d).chunk", floor_num, chunk->pos.x, chunk->pos.y);

    return chunk_save_to_file(chunk, file_name);
}

// TODO add worlds in save system
chunk_t* chunk_load(chunk_t* dest, chunk_pos_t pos, int floor_num) {
    // NULL-check in chunk_load_from_file
    char file_name[64];
    snprintf(file_name, sizeof(file_name),
        "saved/floor_%d/chunk_(%d;%d).chunk", floor_num, pos.x, pos.y);

    return chunk_load_from_file(dest, file_name);
}

void chunk_update(chunk_t* chunk) {
    if (!chunk) return;
}

