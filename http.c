#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "http.h"

#define DATE_LEN 35


const char* month_tostr(int mon)
{
	switch (mon) {
		case 0:
			return "January";
		case 1:
			return "Febuary";
		case 2:
			return "March";
		case 3:
			return "April";
		case 4:
			return "May";
		case 5:
			return "June";
		case 6:
			return "July";
		case 7:
			return "August";
		case 8:
			return "September";
		case 9:
			return "October";
		case 10:
			return "November";
		case 11:
			return "December";
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

void generate_http_message(enum RESPONSE_CODES reponse_code, const char* media_type)
{
	/**
	 * This is assuming a GET request
	 */

	char* date = get_date();


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


	free(date);

}
