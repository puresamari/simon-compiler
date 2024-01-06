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

void addToken(char **val, const char *token)
{
  size_t new_len;
  if (*val == NULL)
  {
    new_len = strlen(token) + 1;
    *val = malloc(new_len);
  }
  else
  {
    new_len = strlen(*val) + strlen(token) + 2;
    *val = realloc(*val, new_len);
    strcat(*val, " ");
  }

  if (*val == NULL)
  {
    perror("Failed to allocate memory in addToken");
    return;
  }

  strcat(*val, token);
}

void addInstructionVariable(InterpretationResult *result, Instruction *instruction, const char *variable, const char *value)
{
  char *nameCopy = strdup(variable);
  char *valueCopy = strdup(value);

  if (nameCopy == NULL || valueCopy == NULL)
  {
    perror("Failed to allocate memory for strings");
    free(nameCopy);
    free(valueCopy);
  }

  InterpretationVariable *newVariables = realloc(result->variables, (result->variablesCount + 1) * sizeof(InterpretationVariable));

  if (newVariables == NULL)
  {
    perror("Failed to reallocate memory");
    free(nameCopy);
    free(valueCopy);
  }

  InterpretationVariable var = {
      .identifier = result->variablesCount,
      .name = strdup(nameCopy),
      .initialValue = strdup(valueCopy)};

  newVariables[result->variablesCount] = var;
  result->variables = newVariables;
  result->variablesCount++;
}

void tokenizeLine(char *line, InterpretationResult *result)
{
  // Tokenize the first token in the line
  char *token = strtok(line, " ");
  if (token == NULL)
    return;

  // Ignore comments
  if (strcmp(token, "//") == 0)
    return;

  Instruction instruction = {};
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

    if (instruction.library == NULL || instruction.function == NULL || (instruction.params != NULL && instruction.params[0].string == NULL))
    {
      fprintf(stderr, "Memory allocation failed in tokenizeLine\n");
      // Free allocated memory
      free(instruction.library);
      free(instruction.function);
      free(instruction.params[0].string);
      free(instruction.params);
      return;
    }

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
  {
    token = strtok(NULL, " ");
    char *declaredVariableName = NULL;
    char *declaredVariableValue = NULL;
    /**
     * 0 = cumulating the variable name
     * 1 = cumulating the initial value
     */
    int dectectionPhase = 0;
    while (token != NULL)
    {
      if (strcmp(token, "is") == 0)
        dectectionPhase++;

      switch (dectectionPhase)
      {
      case 0:
        addToken(&declaredVariableName, token);
        break;
      case 1:
        addToken(&declaredVariableValue, token);
        break;
      }

      token = strtok(NULL, " ");
      tokenIndex++;
    }
    addInstructionVariable(result, &instruction, declaredVariableName, declaredVariableValue);
    break;
  }
  }

  Instruction *newInstructions = realloc(result->instructions, (result->instructionsCount + 1) * sizeof(Instruction));

  if (newInstructions == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    free(instruction.library);
    free(instruction.function);
    return;
  }

  result->instructions = newInstructions;
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
    return result;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1)
  {
    char *newString = strdup(line);
    if (newString == NULL)
    {
      perror("Failed to duplicate line");
      continue;
    }
    stripNewline(newString);
    tokenizeLine(newString, &result);
    free(newString);
  }

  free(line);
  fclose(fp);
  return result;
}
