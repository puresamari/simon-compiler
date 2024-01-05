#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

typedef struct
{
    /**
     * 0 = variable
     * 1 = string
     */
    int type;

    int variable;
    char *string;
} InstructionParameter;

typedef struct
{
    char *name;
    // This should be possible to be of other types aswell
    char *value;
} InstructionVariable;

typedef struct
{
    /**
     * 0 = external lib call
     * 1 = declaration
     */
    int type;
    InstructionParameter *params;
    size_t paramsCount;

    InstructionVariable *variables;
    size_t variablesCount;

    // Only relevant for library instructions
    char *library;
    char *function;
} Instruction;

typedef struct
{
    Instruction *instructions;
    size_t instructionsCount;
} InterpretationResult;

InterpretationResult interpret(const char *inputFile);
InterpretationResult getVariables(InterpretationResult result, int step);

#endif