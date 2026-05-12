#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_states/chunk_loader_state.h"
#include "game.h"
#include "input_handle.h"
#include "game_view.h"
#include "position.h"
#include "direction.h"
#include "util/ansi_wrappers.h"

// todo ? add frame for chunk_screen_t ?
//! fix: chunks rendering still in the middle

#pragma region typedefs
/// local position on `chunk_screen_t`
typedef position_t screen_pos_t;

// generic 2d `int` vector
typedef position_t ivec2;

// byte for bit-fields
typedef unsigned char byte;
#pragma endregion

#pragma region position set
/**
 * @brief Hash set (table) of positions
 * @param size Number of elemets
 * @param cap Max number of elements
 * @param tmbs Dynamic bit field for tombstones (and use-checks)
 * @param data Ptr to malloc'ed array
 * @note `cap` MUST be a power of two
 * @note MUST be cleaned with pos_set_finish
 * @warning Set capacity large enought - it won't dynamiclly resize and will fail
 */
typedef struct {
    unsigned size;
    unsigned cap;
    byte* tmbs;
    position_t* data;
} pos_set;

// rounds up to next power of 2
static inline unsigned next_pow2(unsigned x) {
    if (x <= 1) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

/**
 * @brief Initializes given `pos_set`
 * @param pset `pos_set` to init. Can be garbage
 * @param cap Maximum capacity
 * @return `0` success; `1` invalid params; `2` OOM
 */
static int pos_set_init(pos_set* pset, unsigned cap) {
    if (!pset || cap <= 0) return 1;

    cap = next_pow2(cap);

    position_t* data = malloc(sizeof(position_t) * cap);
    if (!data) return 2;

    byte* tmbs = calloc((cap + 7) / 8, sizeof(byte)); // for partial byte, which shouldn't even be here
    if (!tmbs) {
        free(data);
        return 2;
    }

    for (int i = 0; i < cap; i++) {
        data[i] = pos_none();
    }

    pset->size = 0;
    pset->cap = cap;
    pset->tmbs = tmbs;
    pset->data = data;

    return 0;
}

/**
 * @brief Frees `pos_set`'s resources
 */
static void pos_set_finish(pos_set* pset) {
    if (!pset) return;
    
    pset->size = 0;
    pset->cap = 0;
    
    if (pset->tmbs) {
        free(pset->tmbs);
        pset->tmbs = NULL;
    }
    
    if (pset->data) {
        free(pset->data);
        pset->data = NULL;
    }
}

/**
 * @brief Calculates hash for `position_t`
 */
static inline unsigned hash_pos(position_t pos) {
    unsigned h =
        (unsigned)pos.x * 0x9E3779B1u +
        (unsigned)pos.y * 0x85EBCA77u;

    h ^= h >> 16;
    h *= 0x7FEB352Du;
    h ^= h >> 15;
    h *= 0x846CA68Bu;
    h ^= h >> 16;

    return h;
}

/**
 * @brief Index increment with wrap-around
 */
static inline unsigned idx_advance(pos_set* pset, unsigned idx) {
    return (idx + 1) & (pset->cap - 1); // faster modulo. NEEDS pset->cap = 2**n
}

/**
 * @brief Checks if given index is tombstone ei. ever had data
 * @param pset Valid initialized `pos_set`
 * @param idx Index to check
 */
static inline int tmbs_get(pos_set* pset, unsigned idx) {
    return idx < pset->cap && 
        (pset->tmbs[idx >> 3] & (1u << (idx & 7)));
        // (pset->tmbs[idx / 8] & (1u << (idx % 8))) != 0;
}

/**
 * @brief Sets specified index to tombstone (`1`)
 * @param pset Valid initialized `pos_set`
 * @param idx Index to set
 */
static inline void tmbs_set(pos_set* pset, unsigned idx) {
    if (idx < pset->cap) {
        pset->tmbs[idx >> 3] |= (1u << (idx & 7));
        // pset->tmbs[idx / 8] |= (1u << (idx % 8));
    }
}

/**
 * @brief Sets specified index to free (`0`)
 * @param pset Valid initialized `pos_set`
 * @param idx Index to unset
 * ! prob shoudn't be needed
 */
static inline void tmbs_unset(pos_set* pset, unsigned idx) {
    if (idx < pset->cap) {
        pset->tmbs[idx >> 3] &= ~(1u << (idx & 7));
        // pset->tmbs[idx / 8] &= ~(1u << (idx % 8));
    }
}

/**
 * @brief Inserts element into `pos_set`
 * @param pset Valid `pos_set`
 */
static void pos_set_insert(pos_set* pset, position_t pos) {
    // todo: Grow when too large
    // just temorary
    if (pset->size >= pset->cap) return;

    unsigned idx = hash_pos(pos) & (pset->cap - 1);
    int first_tomb = -1;

    while (tmbs_get(pset, idx))
    {
        if (pos_same(pset->data[idx], pos)) {
            if (first_tomb != -1) {
                pset->data[first_tomb] = pos;
                pset->data[idx] = pos_none();
            }
            return;
        }

        if (first_tomb == -1) {
            first_tomb = idx;
        }

        idx = idx_advance(pset, idx);
    }

    if (first_tomb != -1) {
        idx = (unsigned)first_tomb;
    }
    
    pset->data[idx] = pos;
    tmbs_set(pset, idx);

    pset->size++;
}

static bool pos_set_has(pos_set* pset, position_t pos) {
    // temporary
    if (pset->size >= pset->cap) return false;

    unsigned idx = hash_pos(pos) & (pset->cap - 1);

    while (tmbs_get(pset, idx)) {
        if (pos_same(pset->data[idx], pos)) return true;
        idx = idx_advance(pset, idx);
    }

    return false;
}
#pragma endregion

#pragma region chunk screen
// ? are they needed ?
static bool chunks_changed = false;
static bool cursor_changed = false;
static bool screen_resized = false;
static bool screen_moved = false;

typedef struct chunk_screen_t {
    char* buffer;
    ivec2 dim;
    int buf_line_s; // size of line of buffer (dim.x + 1)
    int buf_size; // precalculated buf_line_s * dim.y
    screen_pos_t offset;
    screen_pos_t cursor;
    void (*cur_color)();
} chunk_screen_t;

static inline void cs_clear(chunk_screen_t* cs) {
    if (!cs || !cs->buffer) return;

    memset(cs->buffer, ' ', cs->buf_size);
    for (int y = 0; y < cs->dim.y; y++) {
        cs->buffer[y * cs->buf_line_s + cs->dim.x] = '\n';
    }
    cs->buffer[cs->buf_size - 1] = '\0';
}

/**
 * @brief Initializes `chunk_screen_t`
 * @note MUST be cleaned with `cs_finish()` 
 * @return `0` success; `1` invalid params, `2` OOM
 */
static int cs_init(chunk_screen_t* cs, ivec2 dim) {
    if (!cs || dim.x <= 0 || dim.y <= 0) return 1;

    int line_s = dim.x + 1;
    int size = line_s * dim.y;

    char* buff = malloc(size * sizeof(char));
    if (!buff) return 2;

    cs->buffer = buff;
    cs->dim = dim;
    cs->buf_line_s = line_s;
    cs->buf_size = size;
    cs->offset = pos_div(dim, -2);
    cs->cursor = pos_new(0, 0);
    cs->cur_color = ansi_bg_bright_blue;

    cs_clear(cs);

    screen_resized = true;

    return 0;
}

static void cs_finish(chunk_screen_t* cs) {
    if (!cs) return;

    if (cs->buffer) {
        free(cs->buffer);
    }

    memset(cs, 0, sizeof(chunk_screen_t));
}

/// @return `0` success; `1` invalid params, `2` OOM
static int cs_resize(chunk_screen_t* cs, ivec2 dim) {
    if (!cs || dim.x <= 0 || dim.y <= 0) return 1;

    int line_s = dim.x + 1;
    int size = line_s * dim.y;

    if (cs->buffer) free(cs->buffer);

    char* buff = malloc(size * sizeof(char));
    if (!buff) return 2;

    cs->buffer = buff;
    cs->dim = dim;
    cs->buf_line_s = line_s;
    cs->buf_size = size;

    cs_clear(cs);

    screen_resized = true;

    return 0;
}

/**
 * @brief Unchecked buffer getter
 * @param cs `Chunk_screen_t` to access
 * @param pos Checked pos (MUST be in bounds)
 * @warning Could access out of bounds - `pos` MUST be checked.
 */
static inline char cs_char_uc(chunk_screen_t* cs, ivec2 pos) {
    return cs->buffer[pos.y * cs->buf_line_s + pos.x];
}

static inline int cs_valid_idx(chunk_screen_t* cs, screen_pos_t pos) {
    return pos.x >= 0 && pos.x < cs->dim.x && pos.y >= 0 && pos.y < cs->dim.y;
}

/**
 * @brief Returns linear index
 * @warning Does not check bounds
 */
static inline unsigned cs_idx(chunk_screen_t* cs, screen_pos_t pos) {
    return pos.y * cs->buf_line_s + pos.x;
}

static inline void cs_draw_chunk(chunk_screen_t* cs, screen_pos_t pos) {
    if (cs_valid_idx(cs, pos)) cs->buffer[cs_idx(cs, pos)] = 'x';
}

static void cs_draw_chunks(chunk_screen_t* cs, pos_set* pset) {
    if (!cs || !pset) return;

    if (chunks_changed || screen_resized || cursor_changed) {
        chunks_changed = false;
        screen_resized = false;
        cursor_changed = false;

        int counter = 0;
        for (int i = 0; i < pset->cap && counter < pset->size; i++) {
            if (!tmbs_get(pset, i) || !pos_valid(pset->data[i])) continue;

            screen_pos_t diff = pos_dif(pset->data[i], cs->offset);
            cs_draw_chunk(cs, diff);
            counter++;
        }
    }
}

static int cs_display(chunk_screen_t* cs) {
    if (!cs) return 1;

    if (fwrite(cs->buffer, sizeof(char), cs->buf_size, stdout) != cs->buf_size) {
        return 2;
    }

    screen_pos_t diff = pos_dif(cs->cursor, cs->offset);
    if (
        diff.x >= 0 && diff.x < cs->dim.x &&
        diff.y >= 0 && diff.y < cs->dim.y
    ) {
        ansi_goto(diff.x + 1, diff.y + 1);
        cs->cur_color();
        putchar(cs_char_uc(cs, diff));
        ansi_reset();
    }

    fflush(stdout);

    return 0;
}

static void cs_translate(chunk_screen_t* cs, ivec2 offset) {
    if (!cs) return;

    cs->offset.x += offset.x;
    cs->offset.y += offset.y;
}

static void cs_cursor_move(chunk_screen_t* cs, ivec2 dir) {
    if (!cs) return;

    cs->cursor.x += dir.x;
    cs->cursor.y += dir.y;
}

#pragma endregion

static chunk_screen_t screen = { 0 };
static pos_set loaded_chunks = { 0 };

int chnkld_init(game_t* game) {
    const int x = 30;
    const int y = 30;

    if (cs_init(&screen, pos_new(x, y)) != 0) return RET_FAIL;

    if (pos_set_init(&loaded_chunks, 512) != 0) return RET_FAIL;

    const char test_r = 1;
    for (int y = -test_r; y <= test_r; y++) {
        for (int x = -test_r; x <= test_r; x++) {
            pos_set_insert(&loaded_chunks, pos_new(x, y));
        }
    }

    return RET_OK;
}

static inline dir4 input_d4(int input) {
    switch (input)
    {
        case INPUT_MOVE_UP: 
        case INPUT_ARROW_UP: return DIR_UP;
        case INPUT_MOVE_LEFT: 
        case INPUT_ARROW_LEFT: return DIR_LEFT;
        case INPUT_MOVE_DOWN:
        case INPUT_ARROW_DOWN: return DIR_DOWN;
        case INPUT_MOVE_RIGHT:
        case INPUT_ARROW_RIGHT: return DIR_RIGHT;
        default: return DIR_NONE;
    }
}

static inline

int chnkld_handle_input(input_handle_t* input, game_t* game, game_view_t* view) {

    switch (input->_char)
    {
    case INPUT_QUIT_PROGRAM: game->should_close = true; break;
    CASE_INPUT_MOVES:
        cs_translate(&screen, get_dir(input_d4(input->_char)));
        view->should_render = true;
        screen_moved = true;
        break;

    CASE_INPUT_ARROWS:
        cs_cursor_move(&screen, get_dir(input_d4(input->_char)));
        view->should_render = true;
        cursor_changed = true;
        break;
    }

    return RET_OK;
}

int chnkld_update(game_t* game) {
    cs_draw_chunks(&screen, &loaded_chunks);

    return RET_OK;
}

int chnkld_render(game_t* game, game_view_t* view) {
    if (!view->should_render) return RET_OK;

    view->should_render = false;

    ansi_clear_screen();
    ansi_goto_home();
    cs_display(&screen);
}

int chnkld_finish(game_t* game) {

    cs_finish(&screen);
    pos_set_finish(&loaded_chunks);

    return RET_OK;
}

game_state_t chunk_loader_gs = {
    .init = chnkld_init,
    .handle_input = chnkld_handle_input,
    .update = chnkld_update,
    .render = chnkld_render,
    .finish = chnkld_finish
};