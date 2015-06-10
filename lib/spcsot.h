/************************************************************************\
*                Copyright (C) Spencer Scott, 2015.                      *
* This program is free software. You may use, modify, and redistrubte it *
* under the terms of the GNU lesser General Public License as published  *
* by the Free Software Foundation. This program is distributed without   *
* any warranty.                                                          *
*************************************************************************/

/* 
 * Commonly used header, includes various other header files and defines
 * a Boolean data type.
 */

#ifndef SPCSOT_H_			/* Prevent accidental double inclusion */
#define SPCSOT_H_

#include <sys/types.h>		/* Type definitions used by many programs */
#include <sys/stat.h>		/* Some system calls require this */
#include <stdio.h>			/* Standard I/O functions */
#include <stdlib.h>			/* Prototypes of commonly used library functions*/
#include <unistd.h>			/* Prototypes for many system calls */
#include <string.h>			/* Commonly used string-handling functions */
#include <errno.h>			/* Declares errno and defines error constants */


#include "error_handle.h"	/* Declares error-handling functions */
#include "get_number.h"		/* Declares functions get number from string */
//#include "get_time.h"		/* Declares functions for printing time */

#define min(a, b)	((a) < (b) ? (a) : (b))
#define max(a, b)   ((a) > (b) ? (a) : (b))


#define MAXLINE	4096		/* max line length */

/*
 * Default file access permissions for new files
 */
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


typedef enum {FALSE, TRUE} Boolean;

#endif  /* SPCSOT_H_ */
