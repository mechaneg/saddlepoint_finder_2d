/// \file file_system.h

#include "file_system.h"

int is_abs_filename (const char *name)
{
  if (*name == '\0')
    return 0;

  if (*name == '/')
    return 1;

  return 0;
}

