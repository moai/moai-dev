/*
  Copyright (C) 2011 Markus Kauppila <markus.kauppila@gmail.com>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <SDL/SDL.h>

#include "logger_helpers.h"

/*!
 * Helper function. Turns the given integer in to a string
 *
 * Note: uses static buffer internally, so the return value
 * isn't valid after the next call of this function. If you
 * want to retain the return value, make a copy of it
 *
 *  \param integer The converted integer
 *  \returns Given integer as string
 */
char *IntToString(const int integer) {
	static char buffer[256]; // malloc might work better
	memset(buffer, 0, sizeof(buffer));

	SDL_snprintf(buffer, sizeof(buffer), "%d", integer);

	return buffer;
}

/*!
 * Helper function. Turns the given integer in to a string in
 * hex format.
 *
 * Note: uses static buffer internally, so the return value
 * isn't valid after the next call of this function. If you
 * want to retain the return value, make a copy of it
 *
 *  \param integer The converted integer
 *  \returns Given integer as string in hex fomat
 */
char *IntToHexString(const Uint64 integer) {
	static char buffer[256]; // malloc might work better
	memset(buffer, 0, sizeof(buffer));

	SDL_snprintf(buffer, sizeof(buffer), "%llX", integer);

	return buffer;
}

/*!
 * Helper function. Turns the given double value in to a string
 *
 * Note: uses static buffer internally, so the return value
 * isn't valid after the next call of this function. If you
 * want to retain the return value, make a copy of it
 *
 * \param decimal The converted double value
 * \returns Given double value as string
 */
char *DoubleToString(const double decimal) {
	static char buffer[256]; // malloc might work better
	memset(buffer, 0, sizeof(buffer));

	SDL_snprintf(buffer, sizeof(buffer), "%.5f", decimal);

	return buffer;
}

/*!
 * Converts unix timestamp to its ascii presentation
 *
 * Note: uses static buffer internally, so the return value
 * isn't valid after the next call of this function. If you
 * want to retain the return value, make a copy of it
 *
 * \param timestamp Timestamp
 * \return Ascii presentation
 */
char *TimestampToString(const time_t timestamp) {
	static char buffer[256];
	memset(buffer, 0, sizeof(buffer));

	time_t copy = timestamp;

	struct tm *local = localtime(&copy);
	strftime(buffer, sizeof(buffer), "%a %Y-%m-%d %H:%M:%S %Z", local);

	return buffer;
}

/*!
 * Converts unix timestamp to its ascii presentation in given format
 *
 * Note: uses static buffer internally, so the return value
 * isn't valid after the next call of this function. If you
 * want to retain the return value, make a copy of it
 *
 * \param timestamp Timestamp
 * \param format Formatting specification such as "%Y%m%d"
 *
 * \return Ascii presentation
 */
char *TimestampToStringWithFormat(const time_t timestamp, char *format) {
	static char buffer[256];
	memset(buffer, 0, sizeof(buffer));

	time_t copy = timestamp;

	struct tm *local = localtime(&copy);
	strftime(buffer, sizeof(buffer), format, local);

	return buffer;
}

/*! Turns all the characters of the given
 * string to lowercase and returns the resulting string.
 *
 * \param string String to be converted
 * \return Newly allocated lower-case version of the given string
 */
char *
ToLowerCase(const char *string)
{
	if(ValidateString(string) == 0) {
		return NULL;
	}

	const int size = SDL_strlen(string);
	char *ret = SDL_malloc(size + 1);
	strncpy(ret, string, size);
	ret[size] = '\0';

	int counter = 0;
	for(; counter < size; ++counter) {
		ret[counter] = tolower(ret[counter]);
	}

	// printf("Debug: %s == %s\n", string, ret);

	return ret;
}

/*!
 * Validates string by checking that given string is not
 * NULL, its length is non-zero etc.
 *
 * \param string Validated string
 * \returns 1 if string is valid, otherwise 0
 */
int
ValidateString(const char *string)
{
	int retVal = 1;

	if(string != NULL) {
		if(SDL_strlen(string) > 0) {
			retVal = 1;
		}

		retVal = 1;
	} else {
		retVal = 0;
	}

	return retVal;
}


