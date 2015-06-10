#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "log_message.h"


static FILE *logfp;			/* Log file stream */

void
logMessage(const char *format, ...)
{
	va_list argList;
	const char *TIMESTAMP_FMT = "%F %X";	/* = YYYY-MM-DD HH:MM:SS*/
#define TS_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS")	/* Includes '\0' */
	char timestamp[TS_BUF_SIZE];
	time_t t;
	struct tm *loc;

	t = time(NULL);
	loc = localtime(&t);
	if (loc == NULL ||
			strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
		fprintf(logfp, "???Unknown time???: ");
	else
		fprintf(logfp, "%s: ", timestamp);

	va_start(argList, format);
	vfprintf(logfp, format, argList);
	fprintf(logfp, "\n");
	va_end(argList);
}

void
keepOneRecord(const char *data)
{
	fprintf(logfp, data);
}

void
logOpen(const char *logFilename)
{
	mode_t m;
	
	m = umask(0777);
	logfp = fopen(logFilename, "a+");
	umask(m);

	/* If opening the log fails we can't display a message... */

	if (logfp == NULL)
		exit(EXIT_FAILURE);

	setbuf(logfp, NULL);

	logMessage("Opened log file");
}

void
logClose(void)
{
	logMessage("Closing log file");
	fclose(logfp);
}
