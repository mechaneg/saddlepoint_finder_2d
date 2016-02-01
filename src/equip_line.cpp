/// \file equip_line.cpp

#include "common_defines.h"
#include "equip_line.h"
#include "report_system.h"



// TODO: refactor. Index is bad idea. Branch on real axe in ok. Try put workplace, param, disp, jt_k in one object
int equip_line::self_build (
  report_system *rep,
  branch_on_real_axe &branch,
  std::vector<complex> &k_branches_workplace,
  const params &param,
  disp_relation &disp,
  jenkins_traub &jt_k)
{
  double min_dist = 0., cur_dist = 0.;

  // check size of branch_on_real_axe and disp and jt_k

  const om_k &real_axe_point = branch.get_points ()[real_axe_index];
  if (disp.calc_k (k_branches_workplace, real_axe_point.om, param, jt_k) < 0)
    {
      rep->print ("Error: cannot compute k for omega = (%5.12lf, %5.12lf).\n",
                  real_axe_point.om.real (), real_axe_point.om.imag ());
      return -1;
    }


  FIX_UNUSED (min_dist);
  FIX_UNUSED (cur_dist);

  return 0;
}
