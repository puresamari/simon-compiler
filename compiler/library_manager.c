#include "library_manager.h"
#include "helpers.h"
#include <stdio.h>

LoadedLibraryFunction *functions = NULL;
int functionsCount = 0;

// Function pointer type definition
typedef char *(*LibraryAssemblyFN)();

void loadLibrary(char *library, char *function)
{
  // I know this inefficient and ugly
  for (int i = 0; i < functionsCount; i++)
  {
    if (strcmp(functions[i].library, library) == 0 && strcmp(functions[i].name, function) == 0)
      return;
    printf("Does not match: %s %s\n", functions[i].library, functions[i].name);
  }

  printf("Loading library '%s' and the function '%s'\n", library, function);
  functions = (LoadedLibraryFunction *)realloc(functions, (functionsCount + 1) * sizeof(LoadedLibraryFunction));

  if (functions == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }

  // Todo: somehow keep the libraries in memory if not all functions have been loaded to optimize compiler time
  const char *libPath = concatStrings(4, getExecutablePath(), "/libraries/lib", library, ".dylib");

  void *handle = dlopen(libPath, RTLD_LAZY);
  if (!handle)
  {
    fprintf(stderr, "Error opening library: %s\n", dlerror());
    return;
  }

  LibraryAssemblyFN getFunction = (LibraryAssemblyFN)dlsym(handle, concatStrings(2, "function_", function));
  if (!getFunction)
  {
    fprintf(stderr, "Error loading function: %s\n", dlerror());
    dlclose(handle);
    return;
  }

  char *assemblyCode = getFunction();
  functions[functionsCount].assembly = malloc(strlen(assemblyCode) * sizeof(char *));
  strcpy(functions[functionsCount].assembly, assemblyCode);

  dlclose(handle);

  functions[functionsCount].library = library;
  functions[functionsCount].name = function;

  functionsCount++;
}

void buildToFile(FILE *fptr)
{
  fprintf(fptr, "// Functions loaded from their libraries: \n");
  for (int i = 0; i < functionsCount; i++)
  {
    fprintf(fptr, "// From library '%s':\n.text\n.align 4\n_%s_%s:\n  %s\n",
            functions[i].library,
            functions[i].library,
            functions[i].name,
            functions[i].assembly);
  }
}
