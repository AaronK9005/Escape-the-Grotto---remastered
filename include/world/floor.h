#pragma once

#include "../position.h"

typedef enum dir4 dir4;
typedef unsigned char dir_set_t;

/**
 * @brief Opaque handle to a `floor_t` object.
 *
 * The concrete definition of `floor_t` is private to the implementation.
 * Users of this API should manage floor objects only through the public
 * functions listed below:
 *
 * - `floor_create()`
 * - `floor_destroy()`
 * - `floor_inline()`
 * - `floor_update()`
 * - `floor_shift_chunks()`
 * - `floor_tile_walkable()`
 * - `floor_tile_seethrough()`
 * - `floor_next_to_wall()`
 */
typedef struct floor_t floor_t;

/**
 * @brief Creates a new `floor_t` object for the given level.
 *
 * Allocates and initializes a new `floor_t` instance representing the
 * specified floor level.
 *
 * @param level Floor level to create.
 * @return Pointer to the newly created floor object, or `NULL` on failure.
 *
 * @note The returned object is owned by the caller and must be destroyed with
 *       `floor_destroy()`.
 * 
 * @note Loads chunks around {0,0}
 */
floor_t* floor_create(const char* game_name, int level);

/**
 * @brief Destroys a floor object.
 *
 * Releases all resources owned by the floor object.
 *
 * @param f Floor object to destroy. May be `NULL`.
 */
void floor_destroy(floor_t* f);

/**
 * @brief Forces saveing on all currently loaded chunks
 * @return `EOF` times number of failed chunk saves, so preferably `0` (on total success), or `1` for invalid `f`
 */
int floor_save(floor_t* f);

void floor_rename(floor_t* f, const char* new_name);

/**
 * @brief Loads chunks around given `chunk_pos`
 */
void floor_set_main_chunk(floor_t* f, chunk_pos_t chunk_pos);

/**
 * @brief Copies a floor object into caller-provided storage.
 *
 * Copies the contents of `f` into `f_buffer`, allowing the floor object
 * to be stored inline instead of separately allocated.
 *
 * @param f_buffer Destination buffer for the copied floor object.
 * @param f Source floor object to copy.
 *
 * @note `f_buffer` must point to valid storage large enough and properly
 *       aligned for a `floor_t`.
 */
void floor_inline(floor_t* f_buffer, const floor_t* f);

/**
 * @brief Updates a floor object by one simulation step.
 *
 * Performs per-update processing for the floor and its internal state.
 *
 * @param f Floor object to update.
 */
void floor_update(floor_t* f);

/**
 * @brief Shifts floor chunks in the given direction.
 *
 * Moves chunk data within the floor according to the specified direction.
 *
 * @param f Floor object to modify.
 * @param d Direction to shift chunks toward.
 */
void floor_shift_chunks(floor_t* f, dir4 d);

/**
 * @brief Returns `tile_id_t` of requested pos
 */
tile_id_t floor_get_tile(floor_t* f, position_t pos);

/**
 * @brief Checks whether a tile can be walked on.
 *
 * @param f Floor object to query.
 * @param pos Tile position to test.
 * @return Nonzero if the tile is walkable, otherwise `0`.
 */
int floor_tile_walkable(floor_t* f, position_t pos);

/**
 * @brief Checks whether a tile can be seen through.
 *
 * @param f Floor object to query.
 * @param pos Tile position to test.
 * @return Nonzero if the tile is see-through, otherwise `0`.
 */
int floor_tile_seethrough(floor_t* f, position_t pos);

/**
 * @brief Returns the direction of a wall adjacent to a tile.
 *
 * Checks tiles adjacent to `pos` for walls and returns the corresponding
 * direction.
 *
 * @param f Floor object to query.
 * @param pos Tile position to inspect.
 * @return Direction of an adjacent wall, or `DIR_NONE` if no wall is
 *         adjacent.
 */
dir_set_t floor_next_to_wall(floor_t* f, position_t pos);