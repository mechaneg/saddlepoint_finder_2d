/// \file model.cpp

#include "model.h"
#include "report_system.h"


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

  if (io_files.create_result_dir (rep) < 0)
    return -1;

  if (init_params_by_file (rep) < 0)
    {
      rep->print ("Error: cannot initialize model by file.\n");
      return -1;
    }

  if (om_k_eval.init (MAX_OM_POLY_DEG, MAX_K_POLY_DEG) < 0)
    return -1;

  return 0;
}

int model::calc_branch_on_real_axe (report_system *rep)
{
  if (real_axe.calc_points (rep, param, om_k_eval) < 0)
    return -1;

  real_axe.calc_real_om_minmax ();

  if (real_axe.calc_extremum_positions (rep) < 0)
    return -1;

  if (real_axe.dump_points (rep, io_files.real_axe_name.toStdString ()) < 0)
    {
      rep->print ("Error: cannot save real_axe info to result file.\n");
      return -1;
    }

  return 0;
}

int model::calc_equip_lines (report_system *rep)
{
  const std::vector<om_k> &points = real_axe.get_points ();
  auto point_iter = points.begin () + 1;    // skip k = (0, 0)
  for (; point_iter != points.end (); point_iter++)
    {
      equip_lines.emplace_back (*point_iter);
      if (equip_lines.back ().self_build_until_real_axe_intersection (rep, real_axe, param, param.d_om, om_k_eval) < 0)
        {
          rep->print ("Cannot build equipotential line from reference k = (%5.12lf,%5.12lf)",
                      point_iter->k.real (), point_iter->k.imag ());
          return -1;
        }
    }

  return 0;
}



