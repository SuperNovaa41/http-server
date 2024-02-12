#ifndef HTTP_H
#define HTTP_H

#define DATE_LEN 35

enum RESPONSE_CODES {
	HTTP_OK = 200,
};

// result must be free'd
char* generate_http_message(enum RESPONSE_CODES reponse_code, const char* media_type, char* body);

const char* parse_http_request(char* request);

#endif
