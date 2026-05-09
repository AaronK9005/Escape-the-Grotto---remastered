#pragma once

#include "map.h"
#include "../position.h"

/**
 * @brief Position type for chunks
 */
typedef position_t chunk_pos_t;

static inline chunk_pos_t glob_to_chunk(position_t pos) {
    int cx = pos.x / MAP_SIZE;
    int cy = pos.y / MAP_SIZE;
    int rx = pos.x % MAP_SIZE;
    int ry = pos.y % MAP_SIZE;

    if (pos.x < 0 && rx != 0) cx--;
    if (pos.y < 0 && ry != 0) cy--;

    return (chunk_pos_t){ cx, cy };
}

/**
 * @brief Self-contained chunk of floor data
 * @param pos Position of chunk in chunk units (MAP_SIZE)
 * @param map 
 */
typedef struct chunk_t {
    chunk_pos_t pos; // position in chunk units
    map_t map; // map of tiles
} chunk_t;

/**
 * @brief Loads chunk from given path
 * @param dest Destination to load to
 * @param path Path to chunk savefile
 * @return SUCCESS: dest; FAIL: NULL
 * @note For loading custom savefile path
 */
chunk_t* chunk_load_from_file(chunk_t* dest, const char* path);

/**
 * @brief Saves chunk to appdata dir
 * @param chunk Chunk to be saved
 * @param path Path to file to save to. NULL saves to standard save dir (appdata)
 * @return SUCCESS: 0; FAIL: EOF
 * @note For custom savefile use
 */
int chunk_save_to_file(chunk_t* chunk, const char* path);

/**
 * @brief Standard chunk loading
 * @param dest Buffer to load to
 * @param chunk_pos Chunk position on the floor
 * @return SUCCESS: dest; FAIL: NULL
 */
chunk_t* chunk_load(chunk_t* dest, chunk_pos_t chunk_pos, const char* game_name, int floor_num);

/**
 * @brief Standard chunk saving
 * @param chunk Chunk to be saved
 * @return `0` on success, `EOF` otherwise
 */
int chunk_save(chunk_t* chunk, const char* game_name, int floor_num);

chunk_t* chunk_generate(chunk_t* dest, chunk_pos_t chunk_pos);

/**
 * @brief Loads different chunk into `dest` or generates, if not generated yet
 * @param dest Buffer to load to
 * @param chunk_pos Chunk position on the floor
 * @param floor_num What floor to load chunk from
 */
static void chunk_load_new(chunk_t* dest, chunk_pos_t chunk_pos, const char* game_name, int floor_num) {
    if (!chunk_load(dest, chunk_pos, game_name, floor_num)) {
        chunk_generate(dest, chunk_pos);
    }
}

//! not implemented yet
void chunk_update(chunk_t* chunk);