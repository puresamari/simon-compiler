#include "library_manager.h"
#include <stdio.h>

LoadedLibraryFunction *functions = NULL;
int functionsCount = 0;

void load(char *library, char *function)
{
  printf("Loading library '%s' and the function '%s'\n", library, function);

  // I know this inefficient and ugly
  for (int i = 0; i < functionsCount; i++)
  {
    if (strcmp(functions[i].library, library) == 0 && strcmp(functions[i].name, function) == 0)
    {
      printf("Library is already loaded\n");
      return;
    }
    printf("Does not match: %s %s\n", functions[i].library, functions[i].name);
  }

  functions = (LoadedLibraryFunction *)realloc(functions, (functionsCount + 1) * sizeof(LoadedLibraryFunction));

  if (functions == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }

  // This is an example for printing, TODO: actually pull it from C

  // x0 holds the value and x1 holds the length
  functions[functionsCount].assembly =
      // Push x0 and x1 onto the stack to preserve em
      "stp x0, x1, [sp, #-16]!\n"

      // Set up arguments for the write syscall
      "mov x0, #1\n"       // File descriptor for stdout
      "ldr x1, [sp]\n"     // Load the value (pointer to string) from stack
      "ldr x2, [sp, #8]\n" // Load the length from stack

      // Perform the write syscall
      "mov x16, #4\n" // System call number for 'write' in ARM64
      "svc #0x80\n"

      // Restore x0 and x1 values from the stack
      "ldp x0, x1, [sp], #16\n"
      "ret\n";

  functions[functionsCount].library = library;
  functions[functionsCount].name = function;

  functionsCount++;
}

char *normalizeAssembly(const char *input)
{
  size_t inputLength = strlen(input);

  char *modifiedString = (char *)malloc((inputLength * 3) + 1);

  if (modifiedString != NULL)
  {
    size_t j = 0;

    for (size_t i = 0; i < inputLength; i++)
    {
      if (input[i] == '\n')
      {
        modifiedString[j++] = '\n';
        modifiedString[j++] = ' ';
        modifiedString[j++] = ' ';
      }
      else
      {
        modifiedString[j++] = input[i];
      }
    }

    modifiedString[j] = '\0';
  }

  return modifiedString;
}

char *build()
{
  char *output = (char *)malloc(1000);

  if (output == NULL)
  {
    printf("Memory allocation for library build output failed\n");
    return NULL;
  }

  strcpy(output, "// Functions loaded from their libraries: \n");

  for (int i = 0; i < functionsCount; i++)
  {

    char *nAssembly = normalizeAssembly(functions[i].assembly);
    char *fnOut = (char *)malloc(
        sizeof(char) * (strlen(functions[i].library) +
                        strlen(functions[i].name) +
                        strlen(nAssembly) +
                        strlen("// From library '':\n_:\n  \n") + 1));

    if (fnOut != NULL)
    {

      sprintf(fnOut,
              "// From library '%s':\n.text\n.align 4\n_%s_%s:\n  %s\n",
              functions[i].library,
              functions[i].library,
              functions[i].name,
              nAssembly);

      output = (char *)realloc(output, strlen(output) + strlen(fnOut) + 1);
      strcat(output, fnOut);

      free(fnOut);
    }
    else
    {
      printf("Memory allocation in lib builder failed\n");
    }
  }

  return output;
}