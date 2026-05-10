#pragma once

#include <stdint.h>

#error Please do not use "date.h" yet. Needs polishing

#define SHORT_DATE_FORMAT "%d.%d.%d"
#define SHORT_DATE_FORMAT_NEGATIVE "%d.%d-%d"
#define DATE_FORMAT "%d. %.3s %d"
#define LONG_DATE_FORMAT "%d. %s %d"

enum {
    DAY_MONDAY =        1 << 0,
    DAY_TUESDAY =       1 << 1,
    DAY_WEDNESDAY =     1 << 2,
    DAY_THURSDAY =      1 << 3,
    DAY_FRIDAY =        1 << 4,
    DAY_SATURDAY =      1 << 5,
    DAY_SUNDAY =        1 << 6,

    DAY_MON = DAY_MONDAY,
    DAY_TUE = DAY_TUESDAY,
    DAY_WEN = DAY_WEDNESDAY,
    DAY_THU = DAY_THURSDAY,
    DAY_FRI = DAY_FRIDAY,
    DAY_SAT = DAY_SATURDAY,
    DAY_SUN = DAY_SUNDAY
};

enum {
    MONTH_JANUARY = 1,
    MONTH_FEBRUARY,
    MONTH_MARCH,
    MONTH_APRIL,
    MONTH_MAY,
    MONTH_JUNE,
    MONTH_JULY,
    MONTH_AUGUST,
    MONTH_SEPTEMBER,
    MONTH_OCTOBER,
    MONTH_NOVEMBER,
    MONTH_DECEMBER,

    MONTH_JAN = MONTH_JANUARY,
    MONTH_FEB = MONTH_FEBRUARY,
    MONTH_MAR = MONTH_MARCH,
    MONTH_APR = MONTH_APRIL,
    // MONTH_MAY = MONTH_MAY,
    MONTH_JUN = MONTH_JUNE,
    MONTH_JUL = MONTH_JULY,
    MONTH_AUG = MONTH_AUGUST,
    MONTH_SEP = MONTH_SEPTEMBER,
    MONTH_OCT = MONTH_OCTOBER,
    MONTH_NOV = MONTH_NOVEMBER,
    MONTH_DEC = MONTH_DECEMBER
};

typedef uint8_t day_t;
typedef uint8_t month_t;
typedef int8_t year_2k_t;

/**
 * @brief Extra small (3B) date struct for some-kind of timestamp in save-files
 * @param mday Day number in month
 * @param month Month number
 * @param year Offset from year 2000 (2026 -> 26; 1999 -> -1)
 */
typedef struct date_t {
    uint8_t mday;
    month_t month;
    year_2k_t year; // years from 2000
} date_t;

// todo: have only one date_str, but more types of display
// ? should each date_str hold it's display method

typedef struct date_str {
    char _[20];
} date_str;

typedef struct date_token {
    date_str str;
    uint8_t str_len;
    uint8_t display_type;
} date_token;

date_str date_to_str(date_t);
