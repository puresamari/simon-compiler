#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int startsWith(const char *str, const char *prefix);
char *substring(const char *str, int start, int length);
void stripNewline(char *str);

#endif
