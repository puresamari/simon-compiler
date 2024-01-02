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

char *concatStrings(int num, ...)
{
  va_list args;
  int length = 0;

  // Initialize the va_list and calculate the total length
  va_start(args, num);
  for (int i = 0; i < num; ++i)
  {
    length += strlen(va_arg(args, char *));
  }
  va_end(args);

  // Allocate memory for the concatenated string
  char *result = malloc(length + 1);
  if (result == NULL)
  {
    perror("Unable to allocate memory for the concatenated string");
    exit(1);
  }
  result[0] = '\0'; // Set the first character to the null terminator

  // Concatenate the strings
  va_start(args, num);
  for (int i = 0; i < num; ++i)
  {
    strcat(result, va_arg(args, char *));
  }
  va_end(args);

  return result;
}

char *getExecutablePath()
{
  char path[PATH_MAX];
  uint32_t size = sizeof(path);

  if (_NSGetExecutablePath(path, &size) != 0)
  {
    fprintf(stderr, "Buffer too small; need size %u\n", size);
    return "";
  }

  char *dir = dirname(path);

  return dir;
}
