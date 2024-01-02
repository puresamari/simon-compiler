#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <mach-o/dyld.h>
#include <limits.h>
#include <libgen.h>
#include <unistd.h>

int startsWith(const char *str, const char *prefix);
char *substring(const char *str, int start, int length);
void stripNewline(char *str);
char *concatStrings(int num, ...);
char *getExecutablePath();

#endif
