#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

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

void loadLibrary(char *library, char *function);
void buildToFile(FILE *fptr);

#endif // LIBRARY_H
