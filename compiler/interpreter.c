#include "interpreter.h"

InterpretationResult interpret(const char *inputFile) {

  printf("Interpreting file \"%s\"...\n", inputFile);
  InterpretationResult result = { .variablesCount = 0, .instructionsCount = 0 };

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

    if (startsWith(newString, "simon says "))
    {
      // Resize the array to accommodate the new string
      result.variables = (char **)realloc(result.variables, (result.variablesCount + 1) * sizeof(char *));

      // Add the new string to the array
      result.variables[result.variablesCount] = substring(newString, strlen("simon says "), strlen(newString) - strlen("simon says ") - 1);
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
      result.instructions[result.instructionsCount].instruction = strdup("says");
      result.instructions[result.instructionsCount].variableIndex = result.variablesCount - 1;

      result.instructionsCount++;
    }
    else
    {
      printf("%s cannot be interpreted \n", newString);
    }

    free(newString); // Free the allocated memory for each line
  }
  
  fclose(fp);

  return result;
}