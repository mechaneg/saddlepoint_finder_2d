/// \file model.cpp

#include "model.h"
#include "params.h"
#include "report_system.h"
#include "jenkins_traub.h"
#include "disp_relation.h"

#include <cstdio>


model::model() {}

model::~model () {}

int model::init_params_by_file (report_system *rep)
{
  std::string filename = io_files.abs_model_name.toStdString ();
  FILE *model_file = fopen (filename.c_str (), "r");
  if (!model_file)
    {
      rep->print ("Error: cannot open model '%s'.\n", filename.c_str ());
      return -1;
    }

  if (param.init_by_file (rep, model_file) < 0)
    {
      fclose (model_file);
      return -1;
    }
  fclose (model_file);

  if (param.check_params (rep) < 0)
    return -1;

  return 0;
}

int model::init_model (report_system *rep, const cmd_params &cmd)
{
  if (io_files.construct_names (rep, cmd) < 0)
    return -1;

  if (init_params_by_file (rep) < 0)
    {
      rep->print ("Error: cannot initialize model by file.\n");
      return -1;
    }

  return 0;
}

int model::calc_branch_on_real_axe (report_system *rep)
{
  /// prepare computational workaround
  jenkins_traub jt_om;
  if (jt_om.init (OM_POLY_DEG) < 0)
    {
      rep->print ("Error: cannot initialize Jenkins-Traub computational method.\n");
      return -1;
    }

  disp_relation disp_rel;
  if (disp_rel.init (OM_POLY_DEG, K_POLY_DEG) < 0)
    {
      rep->print ("Error: cannot initialize computational workaround for dispersion relation");
      return -1;
    }

  /// compute omega values on real axe k
  om_real_axe.emplace_back ();

  std::vector<complex> om_all_branches (0., OM_POLY_DEG);






  return 0;
}






















