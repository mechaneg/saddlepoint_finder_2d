/// \file params.h

#ifndef PARAMS_H
#define PARAMS_H

#include <cstdio>
class report_system;

class params
{
public:
  double M  = 0.;
  double Mw = 0.;
  double D  = 0.;
  double mu = 0.;
  double dx = 0.;

  int M_is_default  = 1;
  int Mw_is_default = 1;
  int D_is_default  = 1;
  int mu_is_default = 1;
  int dx_is_default = 1;

  static const char* M_name;
  static const char* Mw_name;
  static const char* D_name;
  static const char* mu_name;
  static const char* dx_name;

  int init_by_file (report_system *rep, FILE *file);
  int check_params (report_system *rep);
  int init_single_param (const char *param_name, double param);
};

#endif // PARAMS_H

