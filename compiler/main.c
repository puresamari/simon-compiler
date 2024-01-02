#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "writer.h"

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
  InterpretationResult results = interpret(inputFile);

  writeFile(results, outputAssemblyFile);

  return 0;
}
