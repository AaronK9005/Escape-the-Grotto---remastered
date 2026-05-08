#pragma once

#include <stddef.h>

//? doesn't it also need position_t
typedef struct floor_t floor_t;

/**
 * @brief Type for storing floor transitions
 */
typedef struct floor_transition_t {
    floor_t *_f1, *_f2;
} floor_transition_t;

static inline floor_transition_t* transition_init(floor_transition_t* t, floor_t* f1, floor_t* f2) {
    if (!t || !f1 || !f2) return NULL;

    t->_f1 = f1;
    t->_f2 = f2;

    return t;
}

static inline floor_t* transition_get(floor_transition_t* t, floor_t* f) {
    if (!t || !f) return NULL;

    if (f == t->_f1) return t->_f2;
    else if (f == t->_f2) return t->_f1;
    else return NULL;
}
