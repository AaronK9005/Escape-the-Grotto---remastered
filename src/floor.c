#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "world/floor_structure.h"
#include "world/floor.h"
#include "direction.h"

/**
 * @brief Initializes floor's chunks with chunks around `cpos` on given `level`
 * @param level Floor depth to get chunks at
 * @param cpos Position of central chunk
 * @note Does not set `f->level = level`
 *  */ 
static void f_init_chunks(floor_t* const f, int level, chunk_pos_t cpos) {
    const int r = RENDER_DISTANCE;
    int chunk_idx = 0;

    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            chunk_pos_t chunk_pos = {
                .x = cpos.x + j,
                .y = cpos.y + i
            };

            if (NULL == chunk_load(&f->chunk_storage[chunk_idx], chunk_pos, level)) {
                chunk_generate(&f->chunk_storage[chunk_idx], chunk_pos);
            }

            f->chunk[i + r][j + r] = &f->chunk_storage[chunk_idx++];
        }
    }

    f->joker_chunk = *f->chunk[RENDER_DISTANCE][RENDER_DISTANCE];
}

static chunk_t* f_get_chunk_rel(floor_t* f, int dx, int dy) {
    if (!f) return NULL;

    if (abs(dx) <= RENDER_DISTANCE && abs(dy) <= RENDER_DISTANCE) {
        return f->chunk[dy + RENDER_DISTANCE][dx + RENDER_DISTANCE];
    }
    else
    {
        chunk_pos_t requested_cp = pos_sum(f->chunk[RENDER_DISTANCE][RENDER_DISTANCE]->pos, pos_new(dx, dy));

        if (!pos_same(f->joker_chunk.pos, requested_cp))
        {
            chunk_save(&f->joker_chunk, f->level);
            if (NULL == chunk_load(&f->joker_chunk, requested_cp, f->level)) {
                chunk_generate(&f->joker_chunk, requested_cp);
            }
        }

        return &f->joker_chunk;
    }
}

static inline chunk_t* f_main_chunk(floor_t* f) {
    return f ? f->chunk[RENDER_DISTANCE][RENDER_DISTANCE] : NULL;
}

floor_t* floor_create(int level) {
    floor_t* f = malloc(sizeof(floor_t));
    if (!f) return NULL;

    memset(f, 0, sizeof(floor_t));

    f_init_chunks(f, level, pos_new(0, 0));
    f->level = level;

    return f;
}

void floor_destroy(floor_t* f) {
    if (!f) return;

    free(f);
}

void floor_inline(floor_t* f_buffer, floor_t* f) {
    if (!f_buffer || !f) return;

    *f_buffer = *f;
}

void floor_update(floor_t* f) {
    if (!f) return;

    for (int i = 0; i < RENDER_DISTANCE; i++) {
        for (int j = 0; j < RENDER_DISTANCE; j++) {
            chunk_update(f->chunk[i][j]);
        }
    }
}

void floor_shift_chunks(floor_t* f, dir4 d) {
    if (!f || d == DIR_NONE) return;

    switch (d)
    {
    case DIR_UP:
        for (int x = 0; x <= MAX_CHUNK_IDX; x++) {
            chunk_pos_t new_cp = f->chunk[0][x]->pos;
            new_cp.y--;

            chunk_t* temp = f->chunk[MAX_CHUNK_IDX][x];
            chunk_save(temp, f->level);

            for (int y = MAX_CHUNK_IDX; y > 0; y--) {
                f->chunk[y][x] = f->chunk[y - 1][x];
            }
            f->chunk[0][x] = temp;

            chunk_load_new(temp, new_cp, f->level);
        }
        break;

    case DIR_DOWN:
        for (int x = 0; x <= MAX_CHUNK_IDX; x++) {
            chunk_pos_t new_cp = f->chunk[MAX_CHUNK_IDX][x]->pos;
            new_cp.y++;

            chunk_t* temp = f->chunk[0][x];
            chunk_save(temp, f->level);

            for (int y = 0; y < MAX_CHUNK_IDX; y++) {
                f->chunk[y][x] = f->chunk[y + 1][x];
            }
            f->chunk[MAX_CHUNK_IDX][x] = temp;

            chunk_load_new(temp, new_cp, f->level);
        }
        break;

    case DIR_LEFT:
        for (int y = 0; y <= MAX_CHUNK_IDX; y++) {
            chunk_pos_t new_cp = f->chunk[y][0]->pos;
            new_cp.x--;

            chunk_t* temp = f->chunk[y][MAX_CHUNK_IDX];
            chunk_save(temp, f->level);

            for (int x = MAX_CHUNK_IDX; x > 0; x--) {
                f->chunk[y][x] = f->chunk[y][x - 1];
            }
            f->chunk[y][0] = temp;

            chunk_load_new(temp, new_cp, f->level);
        }
        break;

    case DIR_RIGHT:
        for (int y = 0; y <= MAX_CHUNK_IDX; y++) {
            chunk_pos_t new_cp = f->chunk[y][MAX_CHUNK_IDX]->pos;
            new_cp.x++;

            chunk_t* temp = f->chunk[y][0];
            chunk_save(temp, f->level);
            
            for (int x = 0; x < MAX_CHUNK_IDX; x++) {
                f->chunk[y][x] = f->chunk[y][x + 1];
            }
            f->chunk[y][MAX_CHUNK_IDX] = temp;

            chunk_load_new(temp, new_cp, f->level);
        }
        break;
    }
}

tile_id_t floor_get_tile(floor_t* f, position_t pos) {
    if (!f) return 0;

    local_pos_t lp = glob_to_loc(pos);
    chunk_pos_t cp = glob_to_chunk(pos);
    position_t offset = pos_dif(cp, f_main_chunk(f)->pos);
    chunk_t* ch = f_get_chunk_rel(f, POS_UNPACK(offset));

    return ch->map[lp.y][lp.x];
}

int floor_tile_walkable(floor_t* f, position_t pos) {
    return tile_flags[floor_get_tile(f, pos)] && FLAG_WALKABLE;
}

int floor_tile_seethrough(floor_t* f, position_t pos) {
    return !(tile_char[floor_get_tile(f, pos)] && FLAG_OPAQUE);
}

dir_set_t floor_next_to_wall(floor_t* f, position_t pos) {
    if (!f) return DIR_NONE;

    dir_set_t directions = DIR_NONE;

    for (dir8 d = DIR8_FIRST; d <= DIR8_LAST; d <<= 1) {
        if (floor_get_tile(f, pos_sum(pos, get_dir(d))) == TILE_WALL) {
            directions |= d;
        }
    }

    return directions;
}

