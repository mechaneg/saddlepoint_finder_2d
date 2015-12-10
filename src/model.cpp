/// \file model.cpp

#include "model.h"
#include "params.h"
#include "report_system.h"
#include "jenkins_traub.h"
#include "disp_relation.h"

#include <cstdio>
#include <cmath>


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
  branch_real_axe.emplace_back (0., 0.);
  std::vector<complex> om_all_branches (OM_POLY_DEG);

  unsigned int max_count = 1000000;   // max allowed steps
  unsigned int count = 1;
  while (count++ < max_count)
    {
      branch_real_axe.emplace_back ();
      om_k &pair = branch_real_axe.back ();
      pair.k = complex (count * param.dx, 0.);

      if (disp_rel.calc_om (om_all_branches, pair.k, param, jt_om) < 0)
        {
          rep->print ("Error: cannot compute omega for k = (%5.12lf, %5.12lf).\n",
                      pair.k.real (), pair.om.imag ());
          return -1;
        }

      auto iter = om_all_branches.begin ();
      for (; iter != om_all_branches.end (); ++iter)
        {
          if (iter->imag () > IM_OMEGA_MIN &&
              abs (disp_relation::F (*iter, pair.k, param)) < ROOT_SELECT_EPS)
            {
              pair.om = *iter;
              break;
            }
        }
      if (iter == om_all_branches.end ())
        {
          pair.om = complex (0., 0.);
          break;
        }
    }

  /// compute min and max
  re_om_min_real_axe = 0.;
  re_om_max_real_axe = 0.;
  for (const auto &pair : branch_real_axe)
    {
      if (pair.om.real () < re_om_min_real_axe)
        re_om_min_real_axe = pair.om.real ();
      if (pair.om.real () > re_om_max_real_axe)
        re_om_max_real_axe = pair.om.real ();
    }

  const unsigned int min_num_for_extremum_search = 3;
  if (branch_real_axe.size () < min_num_for_extremum_search)
    return 0;

  for (unsigned int i = 0; i < branch_real_axe.size ();)

  return 0;
}






















