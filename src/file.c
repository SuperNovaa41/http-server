#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include "file.h"

int read_file(const char* filename, char** buf)
{
	int err;
	FILE* f;
	long file_size;

	err = access(filename, F_OK);
	if (err != 0) {
		return -1;
	}

	f = fopen(filename, "rb");

	fseek(f, 0L, SEEK_END);
	file_size = ftell(f);
	rewind(f);


	/* allocate the memory */
	*buf = malloc(sizeof(char) * (file_size + 1));
	if (!(*buf)) {
		fclose(f);
		fputs("memory alloc fail\n", stderr);
		exit(EXIT_FAILURE);
	}

	err = fread((*buf), file_size, 1, f);
	if (err != 1) {
		fclose(f);
		free((*buf));
		fputs("reading file failed\n", stderr);
		return -1;
	}

	fclose(f);
	return 0;
}
