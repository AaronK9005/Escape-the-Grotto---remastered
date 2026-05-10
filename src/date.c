#include "date.h"

static const char* months[] = {
    "january",
    "february",
    "march",
    "april",
    "may",
    "june",
    "july",
    "august",
    "september",
    "october",
    "november",
    "december"
};

int snprintf(char *str, size_t, const char *, ...);
int sprintf(char *str, const char *, ...);
int sscanf(const char*, const char*, ...);

date_str date_to_str(date_t d) {
    date_str dstr;
    sprintf(dstr._, DATE_FORMAT, d.mday, months[d.month - 1], d.year + 2000);
    return dstr;
}
