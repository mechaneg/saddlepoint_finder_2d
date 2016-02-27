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

  std::vector<complex> om_roots;
  std::vector<complex> k_roots;

public:

  int init (unsigned max_om_poly_deg, unsigned max_k_poly_deg);
  int calc_k  (const complex &om, const params& param);
  int calc_om (const complex &k, const params& param);

  const std::vector<complex>& get_om_roots ();
  const std::vector<complex>& get_k_roots ();
};

#endif // OM_K_EVALUATOR_H
