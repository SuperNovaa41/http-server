#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#define MIME_TYPES "../src/mime.types"

const char* month_tostr(int mon);
const char* week_day_tostr(int wday);

char* compare_ext_to_mime(const char* ext);


#endif
