/// \file disp_relation.cpp

#include "disp_relation.h"
#include "common_defines.h"
#include "params.h"
#include "jenkins_traub.h"

#include <cmath>

disp_relation::~disp_relation ()
{
  FREE_ARR (om_coeff);
  FREE_ARR (om_coeff_re);
  FREE_ARR (om_coeff_im);
  FREE_ARR (om_zero_re);
  FREE_ARR (om_zero_im);

  FREE_ARR (k_coeff);
  FREE_ARR (k_coeff_re);
  FREE_ARR (k_coeff_im);
  FREE_ARR (k_zero_re);
  FREE_ARR (k_zero_im);
}

int disp_relation::init (unsigned maximum_om_poly_deg, unsigned maximum_k_poly_deg)
{
  max_om_poly_deg = maximum_om_poly_deg;
  max_k_poly_deg  = maximum_k_poly_deg;

  if (check () < 0)
    return -1;

  om_coeff    = new complex[max_om_poly_deg + 1];
  om_coeff_re = new double[max_om_poly_deg + 1];
  om_coeff_im = new double[max_om_poly_deg + 1];
  om_zero_re  = new double[max_om_poly_deg];
  om_zero_im  = new double[max_om_poly_deg];

  k_coeff    = new complex[max_k_poly_deg + 1];
  k_coeff_re = new double[max_k_poly_deg + 1];
  k_coeff_im = new double[max_k_poly_deg + 1];
  k_zero_re  = new double[max_k_poly_deg];
  k_zero_im  = new double[max_k_poly_deg];

  return 0;
}


int disp_relation::check ()
{
  if (max_om_poly_deg == 0)
    return -1;
  if (max_k_poly_deg == 0)
    return -2;
  return 0;
}

int disp_relation::calc_om (std::vector<complex> &om, const complex &k, const params &param, jenkins_traub &jt_om)
{
  unsigned actual_deg = max_om_poly_deg;

  om_coeff[6] = om0 (k, param);   //inverse order in rootfinder
  om_coeff[5] = om1 (k, param);
  om_coeff[4] = om2 (k, param);
  om_coeff[3] = om3 (k, param);
  om_coeff[2] = om4 (k, param);
  om_coeff[1] = om5 (k, param);
  om_coeff[0] = om6 (k, param);

  for (unsigned i = 0; i < actual_deg + 1; i++)
    {
      om_coeff_re[i] = om_coeff[i].real();
      om_coeff_im[i] = om_coeff[i].imag();
    }

  int roots_num = jt_om.cpoly (om_coeff_re, om_coeff_im, actual_deg, om_zero_re, om_zero_im);
  if (roots_num <= 0)
    return -1;

  if (om.size () != static_cast<size_t> (roots_num))
    om.resize (static_cast<size_t> (roots_num));

  for (int i = 0; i < roots_num; i++)
    {
     om[i].real (om_zero_re[i]);
     om[i].imag (om_zero_im[i]);
    }

  return 0;
}

int disp_relation::calc_k (std::vector<complex> &k, const complex &om, const params &param, jenkins_traub &jt_k)
{
  unsigned actual_deg = max_k_poly_deg;

//int& k_num

//if(abs(abs(param.M - param.U) - 1.) > std::numeric_limits<double>::epsilon())
//k_num = 10;
//else if (abs(om) > std::numeric_limits<double>::epsilon())
//k_num = 9;
//else
//k_num = 4;

  k_coeff[0] = k10 (om, param);
  k_coeff[1] = k9 (om, param);
  k_coeff[2] = k8 (om, param);
  k_coeff[3] = k7 (om, param);
  k_coeff[4] = k6 (om, param);
  k_coeff[5] = k5 (om, param);
  k_coeff[6] = k4 (om, param);
  k_coeff[7] = k3 (om, param);
  k_coeff[8] = k2 (om, param);
  k_coeff[9] = k1 (om, param);
  k_coeff[10] = k0 (om, param);

  for (unsigned i = 0; i < actual_deg + 1; i++)
  {
    k_coeff_re[i] = k_coeff[i].real ();	     //inverse order in rootfinder
    k_coeff_im[i] = k_coeff[i].imag ();
  }

  int roots_num = jt_k.cpoly (k_coeff_re, k_coeff_im, actual_deg, k_zero_re, k_zero_im);
  if (roots_num <= 0)
    return -1;

  if (k.size () != static_cast<size_t> (roots_num))
    k.resize (static_cast<size_t> (roots_num));

  for (int i = 0; i < roots_num; i++)
    {
      k[i].real (k_zero_re[i]);
      k[i].imag (k_zero_im[i]);
    }

  return 0;
}

////////////////////////////////////
////// COEFFICIENT FUNCTIONS  //////
////////////////////////////////////

/// omega coefficients
complex disp_relation::om6 (const complex &k, const params &param)
{
  FIX_UNUSED (param);
  FIX_UNUSED (k);
  return complex (-1., 0.);
}

complex disp_relation::om5 (const complex &k, const params &param)
{
  return 2. * k * param.M;
}

complex disp_relation::om4 (const complex &k, const params &param)
{
  return 2. * param.D * pow (k, 4) - pow (param.mu, 2)
         + pow (k, 2) * (2. * pow (param.Mw, 2) - pow (param.M, 2) + 1.);
}

complex disp_relation::om3 (const complex &k, const params &param)
{
  return -4. * k * param.M * (param.D * pow (k, 4) - pow (param.mu, 2) + pow (k, 2) * pow (param.Mw, 2) );
}

complex disp_relation::om2 (const complex &k, const params &param)
{
  return -pow (k, 2) * (pow (param.D, 2) * pow (k, 6) + 6. * pow (param.M, 2) * pow (param.mu, 2) -
         2. * pow (k, 2) * (pow (param.M, 2) - 1.) * pow (param.Mw, 2) + pow (k, 2) * pow (param.Mw, 4) +
         2. * param.D * pow (k, 4) * (1. - pow (param.M, 2) + pow (param.Mw, 2)));
}

complex disp_relation::om1 (const complex &k, const params &param)
{
  return 2. * pow (k, 3) * param.M * (2. * pow (param.M, 2) * pow (param.mu, 2) +
                                      pow (k, 2) * pow (param.D * pow (k, 2) + pow (param.Mw, 2), 2));
}

complex disp_relation::om0 (const complex &k, const params &param)
{
  return -pow (k, 4) * (pow (param.D, 2) * pow (k, 6) * (pow (param.M, 2) - 1.) + pow (param.M, 4) * pow (param.mu, 2) +
         2. * param.D * pow (k, 4) * (pow (param.M, 2) - 1.) * pow (param.Mw, 2) +
                        pow (k, 2) * (pow (param.M, 2) - 1.) * pow (param.Mw, 4));
}

/// k coefficients
complex disp_relation::k10 (const complex &om, const params &param)
{
  FIX_UNUSED (om);
  return -pow (param.D, 2) * (pow (param.M, 2) - 1.);

  /*if(abs(param.M - param.U) <= std::numeric_limits<double>::epsilon())
  return 0.;*/
//return -pow(param.D, 2) * (pow(param.M - param.U, 2) - 1.);
}

complex disp_relation::k9 (const complex &om, const params &param)
{
  //return 2. * pow(param.D, 2) * param.M * om;
  /*if(abs(param.M - param.U) <= std::numeric_limits<double>::epsilon() || \
       abs(om) <= std::numeric_limits<double>::epsilon())
    return 0.;*/

  //return 2. * pow (param.D, 2) * (param.M - param.U) * om;

  return 2. * pow (param.D, 2) * param.M * om;
}

complex disp_relation::k8 (const complex &om, const params &param)
{
  return -param.D * (2. * (pow (param.M, 2) - 1.) * pow (param.Mw, 2) + param.D * pow (om, 2));

//  return param.D * (2. * (pow(param.M - param.U, 2) - 1.) * (pow(param.U, 2) - pow(param.Mw, 2))
//                - param.D * pow(om, 2));
}

complex disp_relation::k7 (const complex &om, const params &param)
{
  return 4. * param.D * param.M * pow (param.Mw, 2) * om;

//  return 4. * param.D * (-param.U + (param.M - param.U) * (pow(param.Mw, 2) +
//    (param.M - 2. * param.U) * param.U) ) * om;
}

complex disp_relation::k6 (const complex &om, const params &param)
{
  return -(pow (param.M, 2) - 1.) * pow (param.Mw, 4) +
    2. * param.D * (pow (param.M, 2) - 1. - pow (param.Mw, 2)) * pow (om, 2);

//  return -(-1. + pow(param.M - param.U, 2)) * pow(param.Mw - param.U, 2) *
//    pow(param.Mw + param.U, 2) + 2. * param.D * (-1. + pow(param.M, 2) - pow(param.Mw, 2) -
//    6. * param.M * param.U + 6. * pow(param.U, 2)) * pow(om, 2);
}

complex disp_relation::k5 (const complex &om, const params &param)
{
  return 2. * param.M * om * (pow(param.Mw, 4) - 2. * param.D * pow(om, 2));

//  return 2. * (param.Mw - param.U) * (param.Mw + param.U) * (-2. * param.U +
//    (param.M - param.U) * (pow(param.Mw, 2) + (2. * param.M - 3. * param.U) * param.U)) *
//    om - 4. * param.D * (param.M - 2. * param.U) * pow(om, 3);
}

complex disp_relation::k4 (const complex &om, const params &param)
{
  return -pow (param.M, 4) * pow (param.mu, 2) - pow (param.Mw, 2) *
         (2. - 2. * pow (param.M, 2) + pow (param.Mw, 2)) * pow (om, 2) + 2. * param.D * pow (om, 4);

//  return -pow(param.mu, 2) * pow(param.M - param.U, 4) + pow(om, 2) *
//    (-pow(param.Mw, 4) + pow(param.U, 2) * (6. - 6. * pow(param.M, 2) +
//    20. * param.M * param.U - 15. * pow(param.U, 2)) + 2. * pow(param.Mw, 2) *
//    (-1. + pow(param.M, 2) - 6. * param.M * param.U + 6. * pow(param.U, 2)) +
//    2. * param.D * pow(om, 2));
}

complex disp_relation::k3 (const complex &om, const params &param)
{
  return 4. * param.M * om * (param.M * param.mu - param.Mw * om) *
    (param.M * param.mu + param.Mw * om);

//  return 4. * om * (pow(param.mu, 2) * pow(param.M - param.U, 3) +
//    (-param.M * pow(param.Mw, 2) + param.U - pow(param.M, 2) * param.U + 2. *
//    pow(param.Mw, 2) * param.U + 5. * param.M * pow(param.U, 2) - 5. * pow(param.U, 3)) *
//    pow(om, 2));
}

complex disp_relation::k2 (const complex &om, const params &param)
{
  return (1. + 2. * pow (param.Mw, 2)) * pow (om, 4) - pow (param.M, 2) *
    (6. * pow (param.mu, 2) * pow (om, 2) + pow (om, 4));

//  return pow(om, 2) * (-6. * pow(param.mu, 2) * pow(param.M - param.U, 2) +
//    (1. - pow(param.M, 2) + 2. * pow(param.Mw, 2) + 10. * param.M * param.U -
//    15. * pow(param.U, 2)) * pow(om, 2));
}

complex disp_relation::k1 (const complex &om, const params &param)
{
  return 2. * param.M * pow (om, 3) * (2. * pow (param.mu, 2) + pow (om, 2));

//  return 4. * pow(param.mu, 2) * (param.M - param.U) * pow(om, 3) + 2. *
//    (param.M - 3. * param.U) * pow(om, 5);
}

complex disp_relation::k0 (const complex &om, const params &param)
{
  return -pow (om, 4) * (pow (param.mu, 2) + pow (om, 2));
}

////////////////////////////////////
////// DISP_RELATION FUNCTIONS  ////
////////////////////////////////////

complex disp_relation::F (const complex &om, const complex &k, const params &param)
{
  return (pow (om, 2) - pow (param.Mw * k, 2) - param.D * pow (k, 4)) * \
                sqrt (pow (k, 2) - pow (om - param.M * k, 2)) + param.mu * pow (om - param.M * k, 2);
}

complex disp_relation::F (const complex &om, const complex &k, double U, const params &param)
{
  return F (om + U * k, k, param);
}

complex disp_relation::F2 (const complex &om, const complex &k, const params &param)
{
  return pow (pow (om, 2) - pow (param.Mw * k, 2) - param.D * pow (k, 4), 2) *
        (pow (k, 2) - pow (om - param.M * k, 2)) - pow (param.mu, 2) * pow (om - param.M * k, 4);
}

complex disp_relation::F2 (const complex &om, const complex &k, double U, const params &param)
{
  return F2 (om + U * k, k, param);
}
