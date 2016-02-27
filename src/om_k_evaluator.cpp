#include "om_k_evaluator.h"


int om_k_evaluator::init (unsigned max_om_poly_deg, unsigned max_k_poly_deg)
{
  if (jt_om.init (max_om_poly_deg) < 0)
    return -1;

  if (jt_k.init (max_k_poly_deg) < 0)
    return -1;

  if (disp_rel.init (max_om_poly_deg, max_k_poly_deg) < 0)
    return -1;

  return 0;
}

int om_k_evaluator::calc_k  (std::vector<complex> &k, const complex &om, const params& param)
{
  if (disp_rel.calc_k (k, om, param, jt_k) < 0)
    return -1;

  return 0;
}

int om_k_evaluator::calc_om (std::vector<complex> &om, const complex &k, const params& param)
{
  if (disp_rel.calc_om (om, k, param, jt_om) < 0)
    return -1;

  return 0;
}
