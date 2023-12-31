// This whole include and ifndef shit is annoying but i'm too stupid to do this differently
#include <stddef.h>

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef ptrdiff_t ssize_t;
#endif

#include "interpreter.h"
#include "library_manager.h"

InterpretationResult interpret(const char *inputFile)
{

  printf("Interpreting file \"%s\"...\n", inputFile);
  InterpretationResult result = {.variablesCount = 0, .instructionsCount = 0};

  FILE *fp = fopen(inputFile, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "Error opening file\n");
    fclose(fp);
    return result;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  printf("Interpreting lines...\n");

  while ((read = getline(&line, &len, fp)) != -1)
  {
    printf("Reading line...\n");
    char *newString = strdup(line);

    char *library = NULL;
    char *function = NULL;
    char *params = NULL;

    char *token = strtok(newString, " ");
    while (token != NULL)
    {
      if (library == NULL)
      {
        library = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(library, token);
      }
      else if (function == NULL)
      {
        function = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(function, token);
      }
      else if (params == NULL)
      {
        params = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(params, token);
      }
      else
      {
        // This is all a bit stupid
        char *n = NULL;
        n = malloc(sizeof(char) * (strlen(params) + 1));
        strcpy(n, params);

        params = malloc(sizeof(char) * (strlen(token) + strlen(n) + 2));
        strcat(params, n);
        strcat(params, " ");
        strcat(params, token);
        stripNewline(params);
      }
      token = strtok(NULL, " ");
    }

    load(library, function);

    // Resize the array to accommodate the new string
    result.variables = (char **)realloc(result.variables, (result.variablesCount + 1) * sizeof(char *));

    // Add the new string to the array
    result.variables[result.variablesCount] = params;
    result.variablesCount++;

    // Resize the array to accommodate the new instruction
    result.instructions = (Instruction *)realloc(result.instructions, (result.instructionsCount + 1) * sizeof(Instruction));

    // Check if memory allocation is successful
    if (result.instructions == NULL)
    {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
    }

    // Set the values for the new instruction
    result.instructions[result.instructionsCount].library = library;
    result.instructions[result.instructionsCount].function = function;
    result.instructions[result.instructionsCount].variableIndex = result.variablesCount - 1;

    result.instructionsCount++;

    free(newString);
  }

  fclose(fp);

  return result;
}