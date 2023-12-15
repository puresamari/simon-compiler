#include "writer.h"

void write(InterpretationResult results, const char *outputAssemblyFile)
{
  printf("Writing out assembly file...\n");

  FILE *fptr = fopen(outputAssemblyFile, "w");
  if (fptr == NULL)
  {
    fprintf(stderr, "Error opening ouput file\n");
    fclose(fptr);
    return;
  }

  fprintf(fptr, ".global _start\n");
  fprintf(fptr, ".align 4\n\n");

  fprintf(fptr, "_start:\n");

  for (int i = 0; i < results.instructionsCount; i++)
  {
    if (strcmp(results.instructions[i].instruction, "says") == 0)
    {
      fprintf(fptr, "  // print var var_%zu\n", results.instructions[i].variableIndex);
      fprintf(fptr, "  mov	x0, #1\n");
      // pointer to the string
      fprintf(fptr, "  adr	X1, var_%zu\n", results.instructions[i].variableIndex);
      // length of the string
      fprintf(fptr, "  mov	X2, #%lu\n", strlen(results.variables[results.instructions[i].variableIndex]) + 1);

      fprintf(fptr, "  mov	X16, #4\n");
      fprintf(fptr, "  svc	#0x80\n\n");
    }
  }

  fprintf(fptr, "  mov x0, #0\n");
  fprintf(fptr, "  mov x16, #1\n");
  fprintf(fptr, "  svc #0x80\n\n");

  for (int i = 0; i < results.variablesCount; i++)
  {
    fprintf(fptr, "var_%d:      .ascii \"%s\\n\"\n", i, results.variables[i]);
  }

  fclose(fptr);
}
