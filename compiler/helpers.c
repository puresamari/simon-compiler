#include "helpers.h"

int startsWith(const char *str, const char *prefix)
{
  return strncmp(str, prefix, strlen(prefix)) == 0;
}

char *substring(const char *str, int start, int length)
{
  char *result = (char *)malloc((length + 1) * sizeof(char));

  if (result == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1); // or handle the error in some way appropriate for your application
  }

  strncpy(result, str + start, length);
  result[length] = '\0'; // Null-terminate the result string

  return result;
}

void stripNewline(char *str)
{
  size_t len = strlen(str);
  if (len > 0 && str[len - 1] == '\n')
  {
    str[len - 1] = '\0'; // Replace newline with null terminator
  }
}