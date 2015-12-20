/// \file branch_on_real_axe.cpp

#include "branch_on_real_axe.h"
#include "jenkins_traub.h"
#include "disp_relation.h"
#include "common_defines.h"
#include "om_k.h"
#include "report_system.h"
#include "params.h"

#include <QFile>
#include <cstdio>

int branch_on_real_axe::calc_points (report_system *rep, const params &param)
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
  points.emplace_back (0., 0.);
  std::vector<complex> om_all_branches (OM_POLY_DEG);

  unsigned int max_count = 1000000;   // max allowed steps
  unsigned int count = 1;
  while (count++ < max_count)
    {
      points.emplace_back ();
      om_k &pair = points.back ();
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

  return 0;
}

void branch_on_real_axe::calc_real_om_minmax ()
{
  re_om_min = 0.;
  re_om_max = 0.;
  for (const auto &pair : points)
    {
      if (pair.om.real () < re_om_min)
        re_om_min = pair.om.real ();
      if (pair.om.real () > re_om_max)
        re_om_max = pair.om.real ();
    }
}

int branch_on_real_axe::calc_extremum_positions (report_system *rep)
{
  const unsigned int min_num_for_extremum_search = 3;
  if (points.size () < min_num_for_extremum_search)
    {
      rep->print ("Error: too few points on real axe: only '%u'.\n",
                  points.size ());
      return -1;
    }

  double prev = points[1].om.imag () - points[0].om.imag ();
  double next = 0.;
  for (unsigned int i = 2; i < points.size (); i++)
    {
      next = points[i].om.imag () - points[i - 1].om.imag ();
      if (prev < 0. && next > 0.)
        im_om_min_positions.push_back (i - 1);
      if (prev > 0. && next < 0.)
        im_om_max_positions.push_back (i - 1);

      prev = next;
    }

  if (!im_om_max_positions.size ())
    {
      rep->print ("Error: no local maximums of Im (omega) on real axe.\n");
      return -1;
    }

  return 0;

}

int branch_on_real_axe::dump_points (const std::string &file_name)
{
  FILE *file = fopen (file_name.c_str (), "w");
  FIX_UNUSED (file);

  return 0;
}
