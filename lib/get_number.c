#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "get_number.h"

static void
gnFail(const char *fname, const char *errmsg, const char *nptr, const char *usrmsg)
{
	fprintf(stderr, "%s error", fname);
	if (usrmsg != NULL)
		fprintf(stderr, ": %s\n", usrmsg);
	fprintf(stderr, ": %s\n", errmsg);
	if (nptr != NULL && *nptr != '\0')
		fprintf(stderr, "        offending text: %s\n", nptr);

	exit(EXIT_FAILURE);
}

static long
getNum(const char *fname, const char *nptr, int flags, const char *msg)
{
	long res;
	char *endptr;
	int base;

	if (nptr == NULL || *nptr == '\0')
		gnFail(fname, "null or empty string", nptr, msg);

	base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
						(flags & GN_BASE_16) ? 16 : 10;
	
	errno = 0;
	res = strtol(nptr, &endptr, base);
	if (errno != 0)
		gnFail(fname, "strol() failed", nptr, msg);

	if (*endptr != '\0')
		gnFail(fname, "nonnumberic character", nptr, msg);

	if ((flags & GN_NONNEG) && res < 0)
		gnFail(fname, "negative value not allowed", nptr, msg);

	if ((flags & GN_GT_0) && res <= 0)
		gnFail(fname, "value must be > 0", nptr, msg);

	return res;
}

long
getLong(const char *nptr, int flags, const char *msg)
{
	return getNum("getLong", nptr, flags, msg);
}

int
getInt(const char *nptr, int flags, const char *msg)
{
	long res;
	
	res = getNum("getInt", nptr, flags, msg);

	if (res > INT_MAX || res < INT_MIN)
		gnFail("getInt", "integer out of range", nptr, msg);

	return (int) res;
}
