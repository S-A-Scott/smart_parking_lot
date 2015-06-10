#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

/* Write a message to the log file. Handle variable length arguement
 * lists, with an initial format string (like printf(3), buf without
 * a trailing newline). Precede each message with a timestamp. */
void logMessage(const char *format, ...);

void keepOneRecord(const char* data);

/* Open the log file 'logFilename' */
void logOpen(const char *logFilename);

/* Close the log file */
void logClose(void);

/* !!!Unimplement!!! */
/*   void readConfigFile(const char *configFilename);   */

#endif
