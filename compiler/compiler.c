#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char *instruction;
  size_t variableIndex;
} Instruction;

int startsWith(const char *str, const char *prefix)
{
  return strncmp(str, prefix, strlen(prefix)) == 0;
}

char *substring(const char *str, int start, int length)
{
  char *result = (char *)malloc((length + 1) * sizeof(char));

  if (result == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1); // or handle the error in some way appropriate for your application
  }

  strncpy(result, str + start, length);
  result[length] = '\0'; // Null-terminate the result string

  return result;
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Use 2 args, 1st arg is input .simon file and 2nd arg is the output binary\n");
    return 0;
  }

  const char *inputFile = argv[1];
  const char *outputAssemblyFile = strcat(argv[2], ".s");

  printf("Simonlang compiler\n - in:      %s\n - out asm: %s\n", inputFile, outputAssemblyFile);

  // Go through lines in file

  printf("Reading input file \"%s\"...\n", inputFile);

  FILE *fp = fopen(inputFile, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "Error opening file\n");
    return 1;
  }

  char **variables = NULL;
  size_t variablesCount = 0;

  Instruction *instructions = NULL;
  size_t instructionsCount = 0;

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
      variables = (char **)realloc(variables, (variablesCount + 1) * sizeof(char *));

      // Add the new string to the array
      variables[variablesCount] = substring(newString, strlen("simon says "), strlen(newString) - strlen("simon says ") - 1);
      variablesCount++;

      // Resize the array to accommodate the new instruction
      instructions = (Instruction *)realloc(instructions, (instructionsCount + 1) * sizeof(Instruction));

      // Check if memory allocation is successful
      if (instructions == NULL)
      {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
      }

      // Set the values for the new instruction
      instructions[instructionsCount].instruction = strdup("says");
      instructions[instructionsCount].variableIndex = variablesCount - 1;

      instructionsCount++;
    }
    else
    {
      printf("%s cannot be interpreted \n", newString);
    }

    free(newString); // Free the allocated memory for each line
  }

  printf("Writing out assembly file...\n");

  FILE *fptr = fopen(outputAssemblyFile, "w");
  if (fptr == NULL)
  {
    fprintf(stderr, "Error opening ouput file\n");
    return 1;
  }

  fprintf(fptr, ".global _start\n");
  fprintf(fptr, ".align 4\n\n");

  fprintf(fptr, "_start:\n");

  for (int i = 0; i < instructionsCount; i++)
  {
    if (strcmp(instructions[i].instruction, "says") == 0)
    {
      fprintf(fptr, "  // print var var_%zu\n", instructions[i].variableIndex);
      fprintf(fptr, "  mov	x0, #1\n");
      // pointer to the string
      fprintf(fptr, "  adr	X1, var_%zu\n", instructions[i].variableIndex);
      // length of the string
      fprintf(fptr, "  mov	X2, #%lu\n", strlen(variables[instructions[i].variableIndex]) + 1);

      fprintf(fptr, "  mov	X16, #4\n");
      fprintf(fptr, "  svc	#0x80\n\n");
    }
  }

  fprintf(fptr, "  mov x0, #0\n");
  fprintf(fptr, "  mov x16, #1\n");
  fprintf(fptr, "  svc #0x80\n\n");

  for (int i = 0; i < variablesCount; i++)
  {
    fprintf(fptr, "var_%d:      .ascii \"%s\\n\"\n", i, variables[i]);
  }

  fclose(fp);
  fclose(fptr);

  return 0;
}
