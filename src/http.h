#ifndef HTTP_H
#define HTTP_H

#define DATE_LEN 35

struct response_file {
	char* mime_type;
	char* content;
	size_t filelen;
};

enum RESPONSE_CODES {
	HTTP_OK = 200,
	HTTP_NOT_FOUND = 404,
	HTTP_NOT_IMPLEMENTED = 501,
};

// result must be free'd
char* generate_http_message(enum RESPONSE_CODES reponse_code, struct response_file* file);

void generate_http_response(char* request, char** response);


#endif
