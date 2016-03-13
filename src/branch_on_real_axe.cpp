/// \file branch_on_real_axe.cpp

#include "branch_on_real_axe.h"
#include "disp_relation.h"
#include "common_defines.h"
#include "om_k.h"
#include "report_system.h"
#include "params.h"

#include <QFile>
#include <cstdio>

int branch_on_real_axe::calc_points (report_system *rep, const params &param, om_k_evaluator &evaluator)
{
  /// compute omega values on real axe k
  points.emplace_back (0., 0.);

  unsigned max_count = 1000000;   // max allowed steps
  unsigned count = 1;
  while (count++ < max_count)
    {
      points.emplace_back ();
      om_k &pair = points.back ();
      pair.k = complex (count * param.dx, 0.);

      if (evaluator.calc_om (pair.k, param) < 0)
        {
          rep->print ("Error: cannot compute omega for k = (%5.12lf, %5.12lf).\n",
                      pair.k.real (), pair.k.imag ());
          return -1;
        }
      const std::vector<complex> &om_all_branches = evaluator.get_om_roots ();

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

int branch_on_real_axe::dump_points (report_system *rep, const std::string &file_name)
{
  if (dump_vector_of_om_k (points, rep, file_name) < 0)
    return -1;
  return 0;
}
