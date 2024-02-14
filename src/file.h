#ifndef FILE_H
#define FILE_H

/**
 * int read_file
 *
 * const char* filename - The path to the file
 * char** buf - The char buffer we will putting the file content into
 *
 * Reads a file.
 *
 * Returns -1 if the file cannot be found. Returns 0 if nothing went wrong. EXIT's if malloc fails.
 *
 * Result must be free'd
 */
int read_file(const char* filename, char** buf);

#endif
