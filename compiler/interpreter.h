#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

typedef struct
{
    // There should be a struct somewhere that contains a reference to the lib and function
    char *library;
    char *function;
    size_t variableIndex;
} Instruction;

typedef struct
{
    char *instruction;
    size_t variableIndex;
} FunctionCompiler;

typedef struct
{
    char **variables;
    size_t variablesCount;

    Instruction *instructions;
    size_t instructionsCount;
} InterpretationResult;

InterpretationResult interpret(const char *inputFile);

#endif