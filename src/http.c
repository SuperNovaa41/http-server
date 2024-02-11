#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "_defines.h"
#include "http.h"

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

char* generate_http_message(enum RESPONSE_CODES response_code, const char* content_type, char* body)
{
	char* ret;
	char* date;
	const char* etag;
	int err;
	size_t body_len;

	/**
	 * This is assuming a GET request
	 */


	// just using the revision number because I don't really have any need to differentiate for now
	etag = VERSION_NUMBER;

	date = get_date();

	body_len = strlen(body);


	/**
	 * HTTP/1.1 200 OK <- response_code
	 * Date:  <- need to generate the date
	 * ETag: <- no idea
	 * Content-Length: <- need the body length
	 * Vary: Accept-Encoding
	 * Content-Type: <- defaults to text/plain
	 *
	 * body here!!!
	 */

	err = asprintf(&ret, "HTTP/1.1 %d OK\n"
			"Date: %s\n"
			"ETag: %s\n"
			"Content-Length: %lu\n"
			"Vary: Accept-Encoding\n"
			"Content-Type: %s\n"
		        "\n"
			"%s", 
			response_code, date, etag, body_len, content_type, body);	

	free(date);

	return ret;
}
