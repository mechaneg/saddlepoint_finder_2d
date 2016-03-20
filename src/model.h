/// \file model.h

#ifndef MODEL_H
#define MODEL_H

#include "input_output_files.h"
#include "params.h"
#include "om_k.h"
#include "branch_on_real_axe.h"
#include "equip_line.h"
#include "om_k_evaluator.h"

#include <complex>

typedef std::complex<double> complex;

struct cmd_params;
class report_system;

class model
{
private:
  input_output_files io_files;
  params param;

  branch_on_real_axe real_axe;
  om_k_evaluator om_k_eval;
  /// equipotential lines
  std::vector<equip_line> equip_lines;    // contains aaaal equipotential lines during finding of saddle point
                                          // for this real_axe.
                                          // refactor it. that's ugly

private:
  int init_params_by_file (report_system *rep);

public:
  model ();
  ~model ();

  int init_model (report_system *rep, const cmd_params &cmd);
  int calc_branch_on_real_axe (report_system *rep);
  int calc_equip_lines (report_system *rep);
};

#endif // MODEL_H
