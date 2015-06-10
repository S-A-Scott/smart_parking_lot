#ifndef READ_LINE_H
#define READ_LINE_H

#include <sys/types.h>

/* Returns number of  bytes copied into buffer(including terminating
 * null byte), or 0 on end-of-file, or -1 on error */
ssize_t readLine(int fd, void *buffer, size_t n);

#endif
