#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#define MIME_TYPES "../src/mime.types"

/**
 * const char* month_tostr
 *
 * int mon - (struct tm) tm_mon
 *
 * Converts mon to a three letter string variant.
 *
 * This is required for the proper HTTP 1.1 RFC date formatting.
 */
const char* month_tostr(int mon);

/**
 * const char* week_day_tostr
 *
 * int wday - (struct tm) tm_wday
 *
 * Converts wday to a three letter string variant.
 *
 * This is required for the proper HTTP 1.1 RFC date formatting.
 */
const char* week_day_tostr(int wday);

/**
 * char* compare_exit_to_mime
 *
 * const char* ext - The extension we're looking for
 *
 * Searches through the MIME_TYPES file and searches for the given extension, and returns
 * its mime type.
 *
 * Result must be free'd
 */
char* compare_ext_to_mime(const char* ext);


#endif
