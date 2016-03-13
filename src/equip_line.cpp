/// \file equip_line.cpp

#include "common_defines.h"
#include "equip_line.h"
#include "report_system.h"
#include "om_k_evaluator.h"

#include <cmath>

static int calc_next_k_on_eqline (
      report_system *rep,
      //result
      om_k &omega_k,
      double om_delta,
      const params &param,
      om_k_evaluator &evaluator)
{
  complex om_next, k_next;
  om_next = omega_k.om + om_delta;

  if (evaluator.calc_k (om_next, param) < 0)
    {
      rep->print ("Error: cannot compute k for omega = (%5.12lf, %5.12lf).\n",
                  om_next.real (), om_next.imag ());
      return -1;
    }
  const std::vector<complex> &k_roots = evaluator.get_k_roots ();

  // there is at least one root
  k_next = k_roots[0];
  double min_dist = abs (k_roots[0] - omega_k.k);
  double cur_dist = 0.;

  for (unsigned i = 1; i < k_roots.size (); i++)
    {
      cur_dist = abs (k_roots[i] - omega_k.k);
      if (cur_dist < min_dist)
        {
          min_dist = cur_dist;
          k_next = k_roots[i];
        }
    }

  omega_k = om_k (om_next, k_next);
  return 0;
}

// Build equipotential line beginning in reference point,
// until total amount of added om_delta (absolute value) reached om_delta_total.

int equip_line::self_build (report_system *rep,
  const params &param,
  double om_delta,        // starting value of om_delta
  double om_delta_total,  // absolute value of Re (om_last) - Re (om_first)
  om_k_evaluator &evaluator)
{
  points.push_back (reference_point);

  om_k prev = reference_point;
  om_k next = prev;

  double distance = 0.;
  double om_added_total = 0.;

  while (std::abs (om_added_total) < om_delta_total)
    {
      if (calc_next_k_on_eqline (rep, next, om_delta, param, evaluator) < 0)
        return -1;

      // optimal distance algorithm
      distance = std::abs (next.k - prev.k);

      while (distance >= param.dx * 0.5) // magic 0.5
        {
          om_delta = om_delta * 0.75;   // magic 0.75

          prev = next;
          if (calc_next_k_on_eqline (rep, next, om_delta, param, evaluator) < 0)
            return -1;
          distance = std::abs (next.k - prev.k);
        }

      om_added_total += om_delta;

      if (distance < param.dx * 0.25)  // magic 0.25
        om_delta = om_delta * 1.5;     // magic 1.5

      points.push_back (next);
      prev = next;
    }

  is_build_flag = true;
  return 0;
}


//  The criteria for stop building eqline are following:
//
//  1) Constraint on Re (om): min Re (om) =< Re(om) <= max Re (om). -> 'intersection::no'
//  2) Intersection with real axe on this complex sheet. -> 'intersection::good'
//  3) Intersection with real axe on the other complex sheet. -> 'intersection::bad'
//
//
//  Also there is empirical algorithm for balancing stepping size of delta Re(omega).
//
//

int equip_line::self_build_until_real_axe_intersection (report_system *rep,
  const branch_on_real_axe &real_axe,
  const params &param,
  double om_delta,
  om_k_evaluator &evaluator)
{
  points.push_back (reference_point);
  const std::vector<om_k> &real_axe_points = real_axe.get_points ();

  om_k prev, next;
  prev = reference_point;
  next = prev;

  double distance = 0.;
  unsigned steps_done = 0;
  while (   prev.k.real () >= real_axe.get_re_om_min ()
         && prev.k.real () <= real_axe.get_re_om_max ())
    {
      if (calc_next_k_on_eqline (rep, next, om_delta, param, evaluator) < 0)
        {
          rep->print ("Cannot build equipotential line, starting from k = (%5.12lf,%5.12lf)",
                      reference_point.k.real (), reference_point.k.imag ());
          return -1;
        }

      // optimal distance algorithm
      distance = std::abs (next.k - prev.k);

      while (distance >= param.dx * 0.5) // magic 0.5
        {
          om_delta = om_delta * 0.75;   // magic 0.75

	  prev = next;
	  calc_next_k_on_eqline (rep, next, om_delta, param, evaluator);
	  distance = std::abs (next.k - prev.k);
	}

      if (distance < param.dx * 0.25)  // magic 0.25
        om_delta = om_delta * 1.5;     // magic 1.5

      // check intersection with real axe
      if (steps_done > 0 && sign (next.k.imag ()) != sign (prev.k.imag ()))
        {
          double k_intersect = prev.k.real () - prev.k.imag () *
                              (next.k - prev.k).real () / (next.k - prev.k).imag ();

          int intersect_index_l = std::floor (k_intersect / param.dx);
          int intersect_index_r = intersect_index_l + 1;

          double im_om_left  = real_axe_points[intersect_index_l].om.imag ();
          double im_om_right = real_axe_points[intersect_index_r].om.imag ();

          // check if the sheet is right
          if (next.om.imag () >= std::min (im_om_left, im_om_right) &&
              next.om.imag () <= std::max (im_om_left, im_om_right))
            state = real_axe_intersection::good;
          else
            state = real_axe_intersection::bad;

          points.push_back (next);
          // let's get out of here
          break;
        }

      points.push_back (next);
      prev = next;
      steps_done++;
    }

  is_build_flag = true;
  return 0;
}
