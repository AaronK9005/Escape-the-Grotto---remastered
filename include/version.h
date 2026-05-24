#pragma once

#include <stdint.h>

#define GAME_VERSION_MAJOR 0
#define GAME_VERSION_MINOR 2
#define GAME_VERSION_PATCH 3

int snprintf(char*, size_t, const char*, ...);

typedef struct {
    uint8_t major;
    uint8_t minor;
    uint16_t patch;
} version_t;

static inline version_t make_vesion(uint8_t major, uint8_t minor, uint16_t patch) {
    return (version_t){
        .major = major,
        .minor = minor,
        .patch = patch
    };
}

static inline version_t game_version() {
    return (version_t){
        .major = GAME_VERSION_MAJOR,
        .minor = GAME_VERSION_MINOR,
        .patch = GAME_VERSION_PATCH
    };
}

static inline const char *version_to_str(version_t v, char *buf, size_t buf_size)
{
    snprintf(buf, buf_size, "%u.%u.%u%s",
             (unsigned)v.major,
             (unsigned)v.minor,
             (unsigned)v.patch
             );

    return buf;
}

static inline int version_compare(version_t a, version_t b)
{
    if (a.major != b.major)
        return (int)a.major - (int)b.major;

    if (a.minor != b.minor)
        return (int)a.minor - (int)b.minor;

    if (a.patch != b.patch)
        return (int)a.patch - (int)b.patch;

    return 0;
}

static inline int version_equal(version_t a, version_t b)
{
    return version_compare(a, b) == 0;
}

static inline int version_is_compatible(version_t game_version, version_t save_version) {
    if (game_version.major != save_version.major)
        return 0;

    return version_compare(save_version, game_version) <= 0;
}