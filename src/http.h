#ifndef HTTP_H
#define HTTP_H

#define DATE_LEN 35

#define DEFAULT_MIME_TYPE "text/plain"

/**
 * struct response_file
 *
 * Struct full of file information
 *
 * char* mime_type - Must be free'd
 * char* content - Must be free'd
 */
struct response_file {
	char* mime_type;
	char* content;
	size_t filelen;
};

/**
 * enum RESPONSE_CODES
 *
 * possible HTTP response codes
 */
enum RESPONSE_CODES {
	HTTP_OK = 200,
	HTTP_NOT_FOUND = 404,
	HTTP_NOT_IMPLEMENTED = 501,
};

/**
 * char* generate_http_message
 *
 * enum RESPONSE_CODES response_code - The response code we're sending with the http response
 * struct response_file* file - The struct containing file information
 *
 * This function will take the given file information, along with the response code
 * and generate an HTTP response following the proper HTTP/1.1 RFC syntax
 *
 * Result must be free'd
 * 
 */
char* generate_http_message(enum RESPONSE_CODES reponse_code, struct response_file* file);

/**
 * void generate_http_response
 *
 * char* request - This is the request we just got from a client
 * char** response - Pointer to the output string
 *
 * This function will take in the request, parse it, and then call all of the appropriate
 * functions we need to generate the proper http response, with proper headers 
 * and a properly formatted body
 *
 * char** response must be free'd
 */
void generate_http_response(char* request, char** response);

/**
 * char* get_mime_type
 * 
 * char* index - the file path (assuming the default GET request file path syntax)
 *
 * Returns a malloc'd pointer to the mime type of the given file
 * Defaults to DEFAULT_MIME_TYPE if the mime type is unknown
 *
 * Result must be free'd
 */
char* get_mime_type(char* index);

/**
 * char* get_date
 *
 * Generates a date string in proper syntax accroding to the HTML/1.1 RFC
 */
char* get_date();

/**
 * enum RESPONSE_CODES setup_file
 *
 * struct response_file* file - The file struct we're going to be filling out
 * char* filepath - The filepath to the file we're reading
 *
 * This will take the file, read it, and fill out the response_file
 * struct. Returns the proper HTTP response code depending on the resulting control flow.
 */
enum RESPONSE_CODES setup_file(struct response_file* file, char* filepath);

#endif
