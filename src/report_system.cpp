#include "report_system.h"

#include <cstdio>
#include <cstdarg>

void report_system::print (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  vprintf (format, args);
  va_end (args);
}
