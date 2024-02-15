#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "_defines.h"
#include "string_helpers.h"
#include "http.h"
#include "file.h"

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

char* generate_http_headers(enum RESPONSE_CODES response_code, struct response_file* file)
{
	char *ret, *date;
	const char* etag;
	int err;
	size_t body_len;

	etag = VERSION_NUMBER;
	date = get_date();

	err = asprintf(&ret, "HTTP/1.1 %d OK\n"
			"Date: %s\n"
			"ETag: %s\n"
			"Content-Length: %lu\n"
			"Vary: Accept-Encoding\n"
			"Content-Type: %s\n",
			response_code, date, etag, file->filelen, file->mime_type);

	if (err == -1) {
		perror("asprintf");
		exit(EXIT_FAILURE);
	}

	free(date);
	return ret;

}

char* generate_http_message(enum RESPONSE_CODES response_code, struct response_file* file)
{
	char* ret;
	char* headers;
	int err;

	headers = generate_http_headers(response_code, file);
	err =  asprintf(&ret, "%s"
		        "\n"
			"%s", 
			headers, file->content);	

	if (err == -1) {
		perror("asprintf");
		exit(EXIT_FAILURE);
	}

	free(headers);
	return ret;
}

enum RESPONSE_CODES setup_file(struct response_file* file, char* filepath)
{
	int err;
	enum RESPONSE_CODES ret;
	char* mime_root;

	if (strcmp(filepath, "") == 0) {
		file->content = "";
		file->filelen = 0;
		return HTTP_RESPONSE_NOT_IMPLEMENTED;
	}

	file->mime_type = get_mime_type(filepath);

	/**
	 * If the mime type is not "text/.." we don't want to 
	 * be reading the file, and just sending it instead
	 */
	mime_root = strtok(file->mime_type, "/");
	if (strcmp(mime_root, "text") != 0) {
		file->content = "";
		file->filelen = 0;
		return HTTP_RESPONSE_OK;
	}

	err = read_file(filepath, &(file->content));
	if (err == -1) { ret = HTTP_RESPONSE_NOT_FOUND;
	
		// force load the 404 file
		read_file(NOTFOUNDFILE, &(file->content));
	} else {
	 	ret = HTTP_RESPONSE_OK;
	}

	puts(file->content);

	file->filelen = strlen(file->content);
	return ret;
}

char* get_mime_type(char* index)
{
	char *file, *ext;
	char* out;

	if (strcmp(index, "") == 0) {
		return DEFAULT_MIME_TYPE;
	}

	file = strrchr(index, '/');
	if (file == NULL) 
		return "";
	ext = strrchr(++file, '.');
	if (ext == NULL)
		return "";

	out = compare_ext_to_mime(ext);
	if (out == NULL) {
		out = malloc(sizeof(char) * strlen(DEFAULT_MIME_TYPE));
		strncpy(out, DEFAULT_MIME_TYPE, strlen(DEFAULT_MIME_TYPE));
	}
	return out;
}

enum HTTP_METHODS get_method(char* type)
{
	if (strcmp(type, "GET") == 0)
		return HTTP_METHOD_GET;
	else if (strcmp(type, "HEAD") == 0)
		return HTTP_METHOD_HEAD;
	else
	 	return HTTP_METHOD_NOT_IMPLEMENTED;
}

void generate_http_response(char* request, char** response)
{
	char *index, *type;
	char *filepath, *filecontent;

	enum RESPONSE_CODES response_code;
	enum HTTP_METHODS method;
	struct response_file file;

	int err;

	type = strtok(request, " ");

	method = get_method(type);

	if (method == HTTP_METHOD_NOT_IMPLEMENTED) {
		response_code = setup_file(&file, "");
		(*response) = generate_http_message(response_code, &file);	
		return;
	}

	// this is the path
	index = strtok(NULL, " ");

	/** First we generate the filename, and then read the file **/
	/**
	 * TODO: this obviously needs to be changed
	 * this doesn't account for any edge cases at all
	 * this will ONLY work for links such as <host>/ <host>/page/, etc
	 *
	 * this also is processing user input so asprintf is the WRONG choice
	 *
	 * (not technically user input, but html headers can be custom made / manipulated to be whatever, so this provides a vector of attack)
	 */
	asprintf(&filepath, "%s%s", HTML_SRC, index);


	response_code = setup_file(&file, filepath);

	if (method == HTTP_METHOD_GET)
		(*response) = generate_http_message(response_code, &file);
	else if (method == HTTP_METHOD_HEAD)
		(*response) = generate_http_headers(response_code, &file);

	puts(*response);

	puts(file.content);

	free(file.content);
	free(file.mime_type);

	free(filepath);
}
