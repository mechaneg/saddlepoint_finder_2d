/// \file model.h

#ifndef MODEL_H
#define MODEL_H

#include "input_output_files.h"
#include "params.h"
#include "om_k.h"

#include <complex>

typedef std::complex<double> complex;

class cmd_params;
class report_system;

class model
{
private:
  input_output_files io_files;
  params param;

  /// real axe info
  std::vector<om_k> branch_real_axe;
  double re_om_min_real_axe = 0.;
  double re_om_max_real_axe = 0.;
  std::vector<unsigned int> im_om_max_real_axe;    // indices of local maximums
  std::vector<unsigned int> im_om_min_real_axe;    // indices of local maximums

private:
  int init_params_by_file (report_system *rep);

public:
  model ();
  ~model ();

  int init_model (report_system *rep, const cmd_params &cmd);
  int calc_branch_on_real_axe (report_system *rep);
  int dump_branch_on_real_axe (report_system *rep);
};

#endif // MODEL_H
