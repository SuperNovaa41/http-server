#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "http.h"
#include "string_helpers.h"

const char* month_tostr(int mon)
{
	switch (mon) {
		case 0:
			return "Jan";
		case 1:
			return "Feb";
		case 2:
			return "Mar";
		case 3:
			return "Apr";
		case 4:
			return "May";
		case 5:
			return "Jun";
		case 6:
			return "Jul";
		case 7:
			return "Aug";
		case 8:
			return "Sep";
		case 9:
			return "Oct";
		case 10:
			return "Nov";
		case 11:
			return "Dec";
		default:
			return "";
	}
}

const char* week_day_tostr(int wday)
{
	switch (wday) {
		case 0:
			return "Sun";
		case 1:
			return "Mon";
		case 2:
			return "Tue";
		case 3:
			return "Wed";
		case 4:
			return "Thu";
		case 5:
			return "Fri";
		case 6:
			return "Sat";
		default:
			return "";
	}

}

char* compare_ext_to_mime(const char* ext)
{
	/**
	 * The layout is (mimetype) (extension) (extension1) (extension2) ...
	 * text/html html
	 */
	FILE* f;
	char *line, *mime, *tok;
	char* out;

	size_t len;
	ssize_t nread;

	int br = 0;

	f = fopen(MIME_TYPES, "r");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	line = NULL;

	ext++; // step past the first character
	
	while ((nread = getline(&line, &len, f)) != -1) {
		mime = strtok(line, " ");
		tok = strtok(NULL, " ");
		while (tok != NULL) {
			if (strcmp(tok, ext) == 0) {
				br = 1;
				break;	
			}
			tok = strtok(NULL, " ");
		}
		if (br == 1)
			break;
	}
	fclose(f);
	
	if (br != 1)
		return DEFAULT_MIME_TYPE;


	out = malloc(sizeof(char) * strlen(mime));
	strncpy(out, mime, strlen(mime) + 1);

	free(line);

	return out;
}
