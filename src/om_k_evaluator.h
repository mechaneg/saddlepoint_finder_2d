#ifndef OM_K_EVALUATOR_H
#define OM_K_EVALUATOR_H

#include "disp_relation.h"
#include "jenkins_traub.h"

class om_k_evaluator
{
private:
  jenkins_traub jt_om;  // for omega evaluation
  jenkins_traub jt_k;   // for k evaluation

  disp_relation disp_rel;  // contains info for evaluating both omega and k

public:

  int init (unsigned max_om_poly_deg, unsigned max_k_poly_deg);
  int calc_k  (std::vector<complex> &k, const complex &om, const params& param);
  int calc_om (std::vector<complex> &om, const complex &k, const params& param);
};

#endif // OM_K_EVALUATOR_H
