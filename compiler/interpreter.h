// interpreter.h
#ifndef INTERPRETER_H
#define INTERPRETER_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

typedef struct
{
    char *instruction;
    size_t variableIndex;
} Instruction;

typedef struct
{
    char **variables;
    size_t variablesCount;

    Instruction *instructions;
    size_t instructionsCount;
} InterpretationResult;

InterpretationResult interpret(const char *inputFile);

#endif