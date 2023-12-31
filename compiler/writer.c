#include "writer.h"

void writeData(InterpretationResult results, FILE *fptr)
{
  for (int i = 0; i < results.variablesCount; i++)
  {
    fprintf(fptr,
            ".align 2\n"
            "var_%d: .ascii \"%s\\n\"\n",
            i, results.variables[i], i, (i == 0) ? 15 : 21);
  }

  fprintf(fptr, "\n");
}

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

  fprintf(fptr,
          ".global _start\n"
          ".align 2\n\n");

  fprintf(fptr, "%s", build());

  fprintf(fptr, "_start:\n");

  for (int i = 0; i < results.instructionsCount; i++)
  {

    int index = results.instructions[i].variableIndex;

    fprintf(fptr,
            "  // Prepare to call %s for var_%d\n"

            "  adr x0, var_%d\n"
            "  mov x1, #%ld\n"
            "  bl _%s_%s\n"

            "  \n",
            results.instructions[i].function,
            index,
            index,
            strlen(results.variables[results.instructions[i].variableIndex]) + 1,
            results.instructions[i].library,
            results.instructions[i].function);
  }

  fprintf(fptr,
          "  mov x0, #0\n"
          "  mov x16, #1\n"
          "  svc #0x80\n\n");

  writeData(results, fptr);
  fclose(fptr);
}
