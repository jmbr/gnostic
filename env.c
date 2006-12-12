/**
 * @file env.c
 * @brief Wrappers for environment variable functions.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#include "logger.h"


void
xputenv(char *string)
{
	if (putenv(string) == -1)
		fatal_error("Unable to declare `%s'\n", string);
}

void
xsetenv(const char *name, const char *value, int overwrite)
{
	if (setenv(name, value, overwrite) == -1)
		fatal_error("Unable to set variable `%s'\n", name);
}
