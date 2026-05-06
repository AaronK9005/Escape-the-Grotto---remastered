#pragma once

#include "map.h"
#include "../position.h"

/**
 * @brief Position type for chunks
 */
typedef position_t chunk_pos_t;

/**
 * @brief Self-contained chunk of floor data
 * @param chunk_pos Position of chunk in chunk units (MAP_SIZE)
 * @param map 
 */
typedef struct chunk_t {
    chunk_pos_t pos;
    map_t map;
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
chunk_t* chunk_load(chunk_t* dest, chunk_pos_t chunk_pos, int floor_num);

/**
 * @brief Standard chunk saving
 * @param chunk Chunk to be saved
 * @return SUCCESS: 0; FAIL: EOF
 */
int chunk_save(chunk_t* chunk, int floor_num);

chunk_t* chunk_generate(chunk_t* dest, chunk_pos_t chunk_pos);

//! not implemented yet
void chunk_update(chunk_t* chunk);

position_t chunk_place_stairs_up(chunk_t* chunk);

position_t chunk_place_stairs_down(chunk_t* chunk);
