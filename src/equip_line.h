/// \file equip_line.h

#ifndef EQUIP_LINE_H
#define EQUIP_LINE_H

#include "branch_on_real_axe.h"
#include "disp_relation.h"
#include "jenkins_traub.h"
#include "om_k.h"
#include "params.h"
#include "real_axe_intersection_state.h"

#include <vector>

class report_system;
typedef std::complex<double> complex;


class equip_line
{
private:
  om_k reference_point;
  std::vector<om_k> points;
  real_axe_intersection state = real_axe_intersection::no;

public:

  equip_line (om_k ref_point) : reference_point (ref_point) {}
  ~equip_line () {}

  inline om_k get_ref_point () const { return reference_point; }
  inline const std::vector<om_k>& get_points () const { return points; }
  inline real_axe_intersection get_state () const { return state; }

  int self_build_until_real_axe_intersection (report_system *rep,
    const branch_on_real_axe &real_axe,
    const params &param,
    double om_delta,
    om_k_evaluator &evaluator);

};


#endif // EQUIP_LINE_H
