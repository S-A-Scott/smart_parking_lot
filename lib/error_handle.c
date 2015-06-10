#include <stdarg.h>
#include "spcsot.h"
#include "error_handle.h"
#include "ename.c.inc"

static const int BUF_SIZE = 1024;


/*
 * Print a message like 'ERROR [ENOENT No such file or directory] userMsg'
 */
static void
printErrMsg(Boolean useErrno, int error, const char *format, va_list ap)
{
	char  userMsg[BUF_SIZE],
		  errText[BUF_SIZE],
		  outputBuf[BUF_SIZE];

	vsnprintf(userMsg, BUF_SIZE -1, format, ap);

	if (useErrno)
		snprintf(errText, BUF_SIZE-1, " [%s %s]", (error > 0 && errno <= MAX_ENAME) ? ename[error] : "?UNKNOWN?",
				strerror(error));
	else
		snprintf(errText, BUF_SIZE, ":");

	snprintf(outputBuf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

	fflush(stdout);				/* In case stdout and stderr are the same */
	fputs(outputBuf, stderr);	
	fflush(NULL);
}


/*
 * Nonfatal error related to a system call
 * Print error message and return
 */
void
errMsg(const char *format, ...)
{
	va_list ap;
	int savedErrno;

	savedErrno = errno;		/* In case we change it here */

	va_start(ap, format);
	printErrMsg(TRUE, errno, format, ap);
	va_end(ap);

	errno = savedErrno;
}

/*
 * Fatal error related to a system call
 * Print error message and terminate
 */
void
errExit(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	printErrMsg(TRUE, errno, format, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

/*
 * Fatal error related to a system call
 * Print a message, dump core, then terminate
 */
void
errAbort(const char *format, ...)
{
	va_list ap;
	
	va_start(ap, format);
	printErrMsg(TRUE, errno, format, ap);
	va_end(ap);

	abort();		/* Dump core and terminate */
}

/*
 * Fatal error unrelated to a system call
 * Print a message and terminate
 */
void
fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	printErrMsg(FALSE, 0, format, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

/*
 * Fatal error unrelated to a system call
 * Print a message and terminate
 */
void
usageErr(const char *format, ...)
{
	va_list ap;

	fflush(stdout);

	fprintf(stderr, "Usage: ");
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	
	fflush(stderr);			/* In case stderr is not line-buffered */
	exit(EXIT_FAILURE);
}
