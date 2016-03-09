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
  unsigned real_axe_index = 0;
  std::vector<om_k> points;
  real_axe_intersection state = real_axe_intersection::no;

public:

  equip_line (unsigned real_axe_index_) : real_axe_index (real_axe_index_) {}
  ~equip_line () {}

  inline unsigned get_real_axe_index () { return real_axe_index; }
  inline const std::vector<om_k>& get_points () { return points; }
  inline real_axe_intersection get_state () { return state; }

  int self_build (report_system *rep,
    const branch_on_real_axe &branch,
    const params &param,
    om_k_evaluator &evaluator);

};


#endif // EQUIP_LINE_H
