#ifndef HTTP_H
#define HTTP_H

#define DATE_LEN 35
#define MAXDATALEN 1000

enum RESPONSE_CODES {
	HTTP_OK = 200,
};

char* generate_http_message(enum RESPONSE_CODES reponse_code, const char* media_type, char* body);


#endif
