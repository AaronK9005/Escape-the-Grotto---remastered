#pragma once

#error Please do not use 'animation.h' yet. Not ready

/**
 * @brief First member animation header
 * 
 * @param anim_type Type of animation
 * @param n_frames Number of frames
 * @param current_f Index of current frame
 *? @param frame Ptr to frame storage
 */
typedef struct animation_t
{
    int anim_type; // type of animation
    unsigned n_frames; // number of frames
    unsigned current_f; // index of current frame
    //void* frame; // frame storage
} animation_t;


enum {
    ANIM_NONE = -1,
    ANIM_COLOR_TEXT,
    ANIM_FRAME,
    ANIM_COUNT
};

typedef struct color_anim_elem_t
{
    float duration;
    int color;
} color_anim_elem_t;

typedef struct color_text_animation_t
{
    animation_t base;
    float time_accum; // time accumulator
    color_anim_elem_t frame[];
} color_text_animation_t;

void update_color_anim(animation_t* anim, float dt) {
    if (!anim || anim->anim_type != ANIM_COLOR_TEXT) return;

    color_text_animation_t* canim = (color_text_animation_t*)anim;

    canim->time_accum += dt;

    float dur = canim->frame[anim->current_f].duration;
    if (dur <= 0.0f) return;

    while (
        // anim->current_f < anim->n_frames &&
        canim->time_accum >= dur
    )
    {
        canim->time_accum -= dur;
        anim->current_f = (anim->current_f + 1) % anim->n_frames;

        dur = canim->frame[anim->current_f].duration;
        if (dur <= 0.0f) break;
    }
    
}

/*
    text color animation (as in 'signal lost')
    frame animation (classic)
*/