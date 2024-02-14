#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "_defines.h"
#include "http.h"
#include "file.h"

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

// Result must be free'd
char* get_date()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char* date;

		
	date = malloc(sizeof(char) * DATE_LEN);
	
	snprintf(date, DATE_LEN, "%s, %02d %s %d %02d:%02d:%02d %s",
			week_day_tostr(tm.tm_wday), tm.tm_mday, month_tostr(tm.tm_mon), 
			tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_zone);

	return date;
}

char* generate_http_message(enum RESPONSE_CODES response_code, struct response_file* file)
{
	char* ret;
	char* date;
	const char* etag;
	int err;
	size_t body_len;

	// just using the revision number because I don't really have any need to differentiate for now
	etag = VERSION_NUMBER;
	date = get_date();

	err = asprintf(&ret, "HTTP/1.1 %d OK\n"
			"Date: %s\n"
			"ETag: %s\n"
			"Content-Length: %lu\n"
			"Vary: Accept-Encoding\n"
			"Content-Type: %s\n"
		        "\n"
			"%s", 
			response_code, date, etag, file->filelen, file->mime_type, file->content);	

	free(date);

	return ret;
}


enum RESPONSE_CODES setup_file(struct response_file* file, char* filepath)
{
	int err;
	enum RESPONSE_CODES ret;
	
	// TODO use mime.types file to find each file type that will associate the mime type
	file->mime_type = "text/html";

	if (strcmp(filepath, "") == 0) {
		file->content = "";
		file->filelen = 0;
		return HTTP_NOT_IMPLEMENTED;
	}

	err = read_file(filepath, &(file->content));
	if (err == -1) {
		ret = HTTP_NOT_FOUND;
	
		// force load the 404 file
		read_file(NOTFOUNDFILE, &(file->content));
	} else {
	 	ret = HTTP_OK;
	}

	file->filelen = strlen(file->content);

	return HTTP_OK;
}

const char* get_mime_type(char* index)
{
	char* file = strrchr(index, '/');
	if (file == NULL) 
		return "";
	char* ext = strrchr(++file, '.');
	if (ext == NULL)
		return "";

	if (strcmp("html", ext) == 0)
		return "text/html";
	return "text/plain";
}

void generate_http_response(char* request, char** response)
{
	char *index, *type;
	char *filepath, *filecontent;

	enum RESPONSE_CODES response_code;
	struct response_file file;

	int err;

	type = strtok(request, " ");

	/**
	 * The server ONLY implements GET for now
	 */
	if (strcmp(type, "GET") != 0) {
		(*response) = generate_http_message(HTTP_NOT_IMPLEMENTED, &file);	
		return;
	}

	// this is the path
	index = strtok(NULL, " ");

	parse_index(index);

	/** First we generate the filename, and then read the file **/
	/**
	 * TODO: this obviously needs to be changed
	 * this doesn't account for any edge cases at all
	 * this will ONLY work for links such as <host>/ <host>/page/, etc
	 */
	asprintf(&filepath, "%s%s/index.html", HTML_SRC, index);


	response_code = setup_file(&file, filepath);

	(*response) = generate_http_message(response_code, &file);

	free(file.content);
	free(filepath);
}
