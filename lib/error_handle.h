#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H

void errMsg(const char *format, ...);


void errExit(const char *format, ...) __attribute__((noreturn));
void errAbort(const char *format, ...) __attribute__((noreturn));
void fatal(const char *format, ...) __attribute__((noreturn));
void usageErr(const char *format, ...) __attribute__((noreturn));

#endif
