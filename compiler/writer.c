#include "writer.h"

void writeData(InterpretationResult results, FILE *fptr)
{
  for (int i = 0; i < results.instructionsCount; i++)
  {
    for (int pI = 0; pI < results.instructions[i].paramsCount; pI++)
    {
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

  for (int vI = 0; vI < results.variablesCount; vI++)
  {
    fprintf(fptr,
            ".align 2\n"
            "var_%d: .ascii \"%s\"\n",
            results.variables[vI].identifier,
            results.variables[vI].initialValue);
  }
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
  buildToFile(fptr);
  fprintf(fptr, "_start:\n");

  for (int i = 0; i < results.instructionsCount; i++)
  {
    // for (int vI = 0; vI < results.instructions[i].variablesCount; vI++)
    // {
    //   fprintf(fptr,
    //           "  // initializing 'var_%s' with value \"%s\"\n",
    //           results.instructions[i].variables[vI].name,
    //           results.instructions[i].variables[vI].value);
    // }

    int reg = 0;
    switch (results.instructions[i].type)
    {
    // external lib call
    case 0:
      fprintf(fptr, "  // Prepare to call %s\n", results.instructions[i].function);
      // The incremental register
      for (int pI = 0; pI < results.instructions[i].paramsCount; pI++)
      {
        // results.instructions[i].variablesCount;
        switch (results.instructions[i].params[pI].type)
        {
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
        default:
          printf("Using %d types in parameters is not implemented yet\n", results.instructions[i].params[pI].type);
          break;
        }
      }
      fprintf(fptr,
              "  bl _%s_%s\n\n",
              results.instructions[i].library,
              results.instructions[i].function);
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
