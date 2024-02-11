#ifndef HTTP_H
#define HTTP_H

enum RESPONSE_CODES {
	HTTP_OK = 200,
};

char* generate_http_message(enum RESPONSE_CODES reponse_code, const char* media_type, const char* body);


#endif
