#include "writer.h"

void writeData(InterpretationResult results, FILE *fptr)
{
  // for (int i = 0; i < results.variablesCount; i++)
  // {
  //   fprintf(fptr,
  //           ".align 2\n"
  //           "var_%d: .ascii \"%s\\n\"\n",
  //           i, results.variables[i]);
  // }

  for (int i = 0; i < results.instructionsCount; i++)
  {

    for (int pI = 0; pI < results.instructions[i].paramsCount; pI++)
    {
      // results.instructions[i].variablesCount;
      switch (results.instructions[i].params[pI].type)
      {
      // param is a variable
      case 0:
        break;
      // param is a string
      case 1:
        fprintf(fptr,
                ".align 2\n"
                "inline_var_%d_%d: .ascii \"%s\"\n",
                i,
                pI,
                results.instructions[i].params[pI].string);
        break;
      }
    }
  }
  fprintf(fptr, "\n");
}

void writeFile(InterpretationResult results, const char *outputAssemblyFile)
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

    fprintf(fptr, "  // Prepare to call %s\n", results.instructions[i].function);

    for (int vI = 0; vI < results.instructions[i].variablesCount; vI++)
    {
      // results.instructions[i].variablesCount;
      // fprintf(fptr,
      //         ".align 2\n"
      //         "var_%s: .ascii ",
      //         results.instructions[i].variables[vI]);
    }

    int reg = 0;
    switch (results.instructions[i].type)
    {
    // external lib call
    case 0:
      // The incremental register
      for (int pI = 0; pI < results.instructions[i].paramsCount; pI++)
      {
        // results.instructions[i].variablesCount;
        switch (results.instructions[i].params[pI].type)
        {
        // param is a variable
        case 0:
          break;
        // param is a string
        case 1:
          fprintf(fptr,
                  "  adr x%d, inline_var_%d_%d\n"
                  "  mov x%d, #%ld\n",
                  reg,
                  i,
                  pI,
                  reg + 1,
                  strlen(results.instructions[i].params[pI].string) + 1);
          reg += 2;
          break;
        }
      }
      fprintf(fptr,
              "  bl _%s_%s\n\n",
              results.instructions[i].library,
              results.instructions[i].function);
      break;
    // declaration
    case 1:
      break;
    }
  }

  fprintf(fptr,
          "  mov x0, #0\n"
          "  mov x16, #1\n"
          "  svc #0x80\n\n");

  writeData(results, fptr);
  fclose(fptr);
}
