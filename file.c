#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

char* read_file(const char* filename)
{
	char* ret;
	int err;
	FILE* f;
	long file_size;

	err = access(filename, F_OK);
	if (err != 0) {
		fprintf(stderr, "File does not exist!\n");
		exit(EXIT_FAILURE);
	}

	f = fopen(filename, "rb");

	fseek(f, 0L, SEEK_END);
	file_size = ftell(f);
	rewind(f);


	/* allocate the memory */
	ret = malloc(sizeof(char) * (file_size + 1));
	if (!ret) {
		fclose(f);
		fputs("memory alloc fail\n", stderr);
		exit(EXIT_FAILURE);
	}

	err = fread(ret, file_size, 1, f);
	if (err != 1) {
		fclose(f);
		free(ret);
		fputs("reading file failed\n", stderr);
		exit(EXIT_FAILURE);
	}

	fclose(f);
	return ret;
}
