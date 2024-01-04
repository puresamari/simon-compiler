// This whole include and ifndef shit is annoying but i'm too stupid to do this differently
#include <stddef.h>

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef ptrdiff_t ssize_t;
#endif

#include "interpreter.h"
#include "library_manager.h"

void determineInstructionTokenType(char *token, InterpretationResult *result, Instruction *instruction)
{
  // Declares variable
  if (strcmp(token, "declare") == 0)
    instruction->type = 1;
  // If not a primitive function, it defaults to library
  else
    instruction->type = 0;
}

void tokenizeLine(char *line, InterpretationResult *result)
{
  char *token = strtok(line, " ");
  if (token == NULL)
    return;

  // Tokenize the first token in the line

  // Ignore comments
  if (strcmp(token, "//") == 0)
    return;

  Instruction instruction = {.variablesCount = 0};
  determineInstructionTokenType(token, result, &instruction);

  int tokenIndex = 0;
  switch (instruction.type)
  {
  case 0:
    while (token != NULL)
    {
      switch (tokenIndex)
      {
      case 0:
        instruction.library = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(instruction.library, token);
        break;
      case 1:
        instruction.function = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(instruction.function, token);
        break;
      // TODO: Don't assume the parameters are strings
      default:
        if (instruction.params == NULL)
        {
          instruction.params = malloc(sizeof(InstructionParameter));
          instruction.params[0].type = 1;
          instruction.params[0].string = malloc(sizeof(char) * (strlen(token) + 1));
          strcpy(instruction.params[0].string, token);
          instruction.paramsCount = 1;
        }
        // This is all a bit stupid
        else
        {
          char *n = concatStrings(3, instruction.params[0].string, " ", token);
          instruction.params[0].string = malloc(sizeof(char) * (strlen(n) + 1));
          strcpy(instruction.params[0].string, n);
        }
      }
      token = strtok(NULL, " ");
      tokenIndex++;
    }
    loadLibrary(instruction.library, instruction.function);

    // Resize the array to accommodate the new instruction
    result->instructions = (Instruction *)realloc(result->instructions, (result->instructionsCount + 1) * sizeof(Instruction));

    // Check if memory allocation is successful
    if (result->instructions == NULL)
    {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
    }
    break;
  // Declaration
  case 1:
    printf("! Variable declaration is not implemented yet !\n");
    return;
  }

  result->instructions[result->instructionsCount] = instruction;

  result->instructionsCount++;
}

InterpretationResult interpret(const char *inputFile)
{

  printf("Interpreting file \"%s\"...\n", inputFile);
  InterpretationResult result = {.instructionsCount = 0};

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

  while ((read = getline(&line, &len, fp)) != -1)
  {
    char *newString = strdup(line);
    stripNewline(newString);
    tokenizeLine(newString, &result);
    free(newString);
  }

  fclose(fp);

  return result;
}