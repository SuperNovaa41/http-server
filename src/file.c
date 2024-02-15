#define _FILE_OFFSET_BITS 64

#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include "file.h"

int read_file(const char* filename, char** buf)
{
	int err;
	FILE* f;
	long long file_size;

	err = access(filename, F_OK);
	if (err != 0)
		return -1;

	f = fopen(filename, "rb");

	fseeko(f, 0L, SEEK_END);
	file_size = ftell(f);

	/**
	 * TODO: im not sure what is causing this but, if this isn't here, sometimes
	 * garbage memory will appear at the end of the buffer
	 */
	file_size -= 2;

	rewind(f);

	/* allocate the memory */
	*buf = malloc(file_size);
	if (!(*buf)) {
		fclose(f);
		fputs("memory alloc fail\n", stderr);
		exit(EXIT_FAILURE);
	}

	err = fread((*buf), file_size, sizeof(char), f);
	if (err != 1) {
		fclose(f);
		free((*buf));
		fputs("reading file failed\n", stderr);
		return -1;
	}

	fclose(f);
	return 0;
}
