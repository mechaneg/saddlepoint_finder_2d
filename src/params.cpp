/// \file params.cpp

#include "params.h"
#include "common_defines.h"
#include "report_system.h"

#include <cstring>

const char* params::M_name  = "M";
const char* params::Mw_name = "Mw";
const char* params::D_name  = "D";
const char* params::mu_name = "mu";
const char* params::dx_name = "dx";
const char* params::d_om_name = "d_om";

int params::check_params (report_system *rep)
{
  ///// M
  if (M_is_default)
    {
      rep->print ("Error: 'M' parameter was not set.\n");
      return -1;
    }
  else if (M < MIN_FOR_COMPARE)
    {
      rep->print ("Error: 'M' parameter should be positive.\n");
      return -1;
    }

  ///// Mw
  if (Mw_is_default)
    {
      rep->print ("Error: 'Mw' parameter was not set.\n");
      return -1;
    }
  else if (Mw < 0.)
    {
      rep->print ("Error: 'Mw' parameter shouldn't be negative.\n");
      return -1;
    }

  ///// D
  if (D_is_default)
    {
      rep->print ("Error: 'D' parameter was not set.\n");
      return -1;
    }
  else if (D < 0.)
    {
      rep->print ("Error: 'D' parameter shouldn't be negative.\n");
      return -1;
    }

  ///// mu
  if (mu_is_default)
    {
      rep->print ("Error: 'mu' parameter was not set.\n");
      return -1;
    }
  else if (mu < MIN_FOR_COMPARE)
    {
      rep->print ("Error: 'mu' parameter should be positive.\n");
      return -1;
    }

  ///// dx
  if (dx_is_default)
    {
      rep->print ("Error: 'dx' parameter was not set.\n");
      return -1;
    }
  else if (dx < MIN_FOR_COMPARE)
    {
      rep->print ("Error: 'dx' parameter should be positive.\n");
      return -1;
    }

  ///// d_om
  if (d_om_is_default)
    {
      rep->print ("Error: 'd_om' parameter was not set.\n");
      return -1;
    }
  else if (d_om < MIN_FOR_COMPARE)
    {
      rep->print ("Error: 'd_om' parameter should be positive.\n");
      return -1;
    }

  return 0;
}

int params::init_single_param (const char *param_name, double param)
{
  if (!strcmp (param_name, M_name))
    {
      M = param;
      M_is_default = 0;
    }
  else if (!strcmp (param_name, Mw_name))
    {
      Mw = param;
      Mw_is_default = 0;
    }
  else if (!strcmp (param_name, D_name))
    {
      D = param;
      D_is_default = 0;
    }
  else if (!strcmp (param_name, mu_name))
    {
      mu = param;
      mu_is_default = 0;
    }
  else if (!strcmp (param_name, dx_name))
    {
      dx = param;
      dx_is_default = 0;
    }
  else if (!strcmp (param_name, d_om_name))
    {
      d_om = param;
      d_om_is_default = 0;
    }
  else
    return -1;

  return 0;
}


int params::init_by_file (report_system *rep, FILE *file)
{
  if (!rep || !file)
    return -1;

  char buf[CHAR_BUF_LEN] = {};
  const int par_len = 32;
  char   param_name[par_len];
  double param = 0.;

  char *res_string = 0;
  unsigned int line_num = 0;

  rep->print ("Info: start reading of input file.\n");

  while (true)
    {
      /// read string to buffer
      if (!(res_string = fgets (buf, CHAR_BUF_LEN, file)))
        {
          if (ferror (file))
            {
              rep->print ("Error: cannot read '%u' line of input file.\n"
                          "\"%s\".\n", line_num, res_string);
              return -1;
            }
          if (feof (file))
            break;
        }

      /// parse buffer
      if (buf[0] == '-' && buf[1] == '-')   // this is comment. skip it
        continue;
      if (sscanf (buf, "%s %lf", param_name, &param) != 2)
        {
          rep->print ("Error: cannot read parameter name and parameter on "
                      "'%u' line of input file.\n", line_num);
          continue;
        }

      if (init_single_param (param_name, param) < 0)
        rep->print ("Error: cannot parse parameter name '%s' on line '%u'.\n",
                    param_name, line_num);

      line_num++;
    }

  rep->print ("Info: end reading of input file.\n");
  return 0;
}
