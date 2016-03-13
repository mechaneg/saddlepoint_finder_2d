/// \file om_k.cpp

#include "om_k.h"
#include "report_system.h"

int dump_vector_of_om_k (const std::vector<om_k> &points, report_system *rep, const std::string &file_name)
{
  FILE *file = nullptr;
  if (!(file = fopen (file_name.c_str (), "w")))
    {
      rep->print ("Error: cannot open file '%s'.\n", file_name.c_str ());
      return -1;
    }

  fprintf (file, "Re(k)\tIm(k)\tRe(om)\tIm(om):\n");
  for (const auto &pair : points)
    {
      fprintf (file, "%5.12lf\t%5.12lf\t%5.12lf\t%5.12lf\n",
               pair.k.real (), pair.k.imag (), pair.om.real (), pair.om.imag ());
    }

  fclose (file);
  return 0;
}

