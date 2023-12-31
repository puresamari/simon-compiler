#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char *name;
  char *assembly;
} LibraryFunction;

typedef struct
{
  LibraryFunction *functions;
} Library;

typedef struct
{
  char *library;
  char *name;
  char *assembly;
} LoadedLibraryFunction;

void load(char *library, char *function);
char *build();

#endif // LIBRARY_H
