/// \file disp_rel.cpp

#include "disp_rel.h"
#include "common_defines.h"
#include "params.h"

#include <cmath>

void disp::calc_om (std::valarray<complex<double>> &om, const complex &k, const params &param)
{
  static complex<double>* om_coeff = new complex<double>[OM_POLY_DEG + 1];
  static double* om_coeff_re = new double[OM_POLY_DEG + 1];
  static double* om_coeff_im = new double[OM_POLY_DEG + 1];
  static double* om_zero_re  = new double[OM_POLY_DEG];
  static double* om_zero_im  = new double[OM_POLY_DEG];

  int i, j;

  if (om.size () != OM_POLY_DEG)
    {
      std::cout << "size of input valarray om != om_poly_deg\n";
      std::cout << "k = " << k << ". params = (" << param.Mw << " " << param.M << " "\
                << param.D << " " << param.mu << ")\n";
      std::cin.get();
      std::abort();
    }

//////////////

  om_coeff[6] = om0 (k, param);		//inverse order in rootfinder
  om_coeff[5] = om1 (k, param);
  om_coeff[4] = om2 (k, param);
  om_coeff[3] = om3 (k, param);
  om_coeff[2] = om4 (k, param);
  om_coeff[1] = om5 (k, param);
  om_coeff[0] = om6 (k, param);

  for (i = 0; i < OM_POLY_DEG + 1; i++)
    {
      om_coeff_re[i] = om_coeff[i].real();
      om_coeff_im[i] = om_coeff[i].imag();
    }

  j = JT::cpoly(om_coeff_re, om_coeff_im, OM_POLY_DEG, om_zero_re, om_zero_im);

//////////////

  if(j != OM_POLY_DEG)
    {
      std::cout << "number of roots with JT = " << j << ". which != om_poly_degree.\n";
      std::cout << "k = " << k << ". params = (" << param.Mw << " " << param.M << " "\
                << param.D << " " << param.mu << ")\n";
      std::cin.get();
      std::abort();
    }

  for(i = 0; i < OM_POLY_DEG; i++)
    {
     om[i].real(om_zero_re[i]);
     om[i].imag(om_zero_im[i]);
    }
}


////////////////////////////////////
////// COEFFICIENT FUNCTIONS  //////
////////////////////////////////////

complex disp::om6 (const complex &k, const params &param)
{
  FIX_UNUSED (param);
  FIX_UNUSED (k);
  return complex (-1., 0.);
}

complex disp::om5 (const complex &k, const params &param)
{
  return 2. * k * param.M;
}

complex disp::om4 (const complex &k, const params &param)
{
  return 2. * param.D * pow (k, 4) - pow (param.mu, 2)
         + pow (k, 2) * (2. * pow (param.Mw, 2) - pow (param.M, 2) + 1.);
}

complex disp::om3 (const complex &k, const params &param)
{
  return -4. * k * param.M * (param.D * pow (k, 4) - pow (param.mu, 2) + pow (k, 2) * pow (param.Mw, 2) );
}

complex disp::om2 (const complex &k, const params &param)
{
  return -pow (k, 2) * (pow (param.D, 2) * pow (k, 6) + 6. * pow (param.M, 2) * pow (param.mu, 2) -
         2. * pow (k, 2) * (pow (param.M, 2) - 1.) * pow (param.Mw, 2) + pow (k, 2) * pow (param.Mw, 4) +
         2. * param.D * pow (k, 4) * (1. - pow (param.M, 2) + pow (param.Mw, 2)));
}

complex disp::om1 (const complex &k, const params &param)
{
  return 2. * pow (k, 3) * param.M * (2. * pow (param.M, 2) * pow (param.mu, 2) +
                                      pow (k, 2) * pow (param.D * pow (k, 2) + pow (param.Mw, 2), 2));
}

complex disp::om0 (const complex &k, const params &param)
{
  return -pow (k, 4) * (pow (param.D, 2) * pow (k, 6) * (pow (param.M, 2) - 1.) + pow (param.M, 4) * pow(param.mu, 2) +
         2. * param.D * pow (k, 4) * (pow (param.M, 2) - 1.) * pow (param.Mw, 2) +
                        pow (k, 2) * (pow (param.M, 2) - 1.) * pow (param.Mw, 4));
}

complex disp::k10 (const complex &om, const params &param)
{
  //return -pow(param.D, 2) * (pow(param.M, 2) - 1.);
  /*if(abs(param.M - param.U) <= std::numeric_limits<double>::epsilon())
  return 0.;*/

  return -pow(param.D, 2) * (pow(param.M - param.U, 2) - 1.);
}

complex disp::k9 (const complex &om, const params &param)
{
  //return 2. * pow(param.D, 2) * param.M * om;
  /*if(abs(param.M - param.U) <= std::numeric_limits<double>::epsilon() || \
       abs(om) <= std::numeric_limits<double>::epsilon())
    return 0.;*/

  return 2. * pow (param.D, 2) * (param.M - param.U) * om;
}

complex disp::k8 (const complex &om, const params &param)
{
	//return -param.D * (2. * (pow(param.M, 2) - 1.) * pow(param.Mw, 2) + param.D * pow(om, 2));

	return param.D * (2. * (pow(param.M - param.U, 2) - 1.) * (pow(param.U, 2) - pow(param.Mw, 2)) \
		- param.D * pow(om, 2));
}

complex disp::k7 (const complex &om, const params &param)
{
  //return 4. * param.D * param.M * pow(param.Mw, 2) * om;

  return 4. * param.D * (-param.U + (param.M - param.U) * (pow(param.Mw, 2) + \
    (param.M - 2. * param.U) * param.U) ) * om;
}

complex disp::k6 (const complex &om, const params &param)
{
/*return -(pow(param.M, 2) - 1.) * pow(param.Mw, 4) + \
        2. * param.D * (pow(param.M, 2) - 1. - pow(param.Mw, 2)) * pow(om, 2);*/

  return -(-1. + pow(param.M - param.U, 2)) * pow(param.Mw - param.U, 2) * \
    pow(param.Mw + param.U, 2) + 2. * param.D * (-1. + pow(param.M, 2) - pow(param.Mw, 2) - \
    6. * param.M * param.U + 6. * pow(param.U, 2)) * pow(om, 2);
}

complex disp::k5 (const complex &om, const params &param)
{
  //return 2. * param.M * om * (pow(param.Mw, 4) - 2. * param.D * pow(om, 2));

  return 2. * (param.Mw - param.U) * (param.Mw + param.U) * (-2. * param.U + \
    (param.M - param.U) * (pow(param.Mw, 2) + (2. * param.M - 3. * param.U) * param.U)) * \
    om - 4. * param.D * (param.M - 2. * param.U) * pow(om, 3);
}

complex disp::k4 (const complex &om, const params &param)
{
  /*return -pow(param.M, 4) * pow(param.mu, 2) - pow(param.Mw, 2) * \
                (2. - 2. * pow(param.M, 2) + pow(param.Mw, 2)) * pow(om, 2) + 2. * param.D * pow(om, 4);*/

  return -pow(param.mu, 2) * pow(param.M - param.U, 4) + pow(om, 2) * \
    (-pow(param.Mw, 4) + pow(param.U, 2) * (6. - 6. * pow(param.M, 2) + \
    20. * param.M * param.U - 15. * pow(param.U, 2)) + 2. * pow(param.Mw, 2) * \
    (-1. + pow(param.M, 2) - 6. * param.M * param.U + 6. * pow(param.U, 2)) + \
    2. * param.D * pow(om, 2));
}

complex disp::k3 (const complex &om, const params &param)
{
  /*return 4. * param.M * om * (param.M * param.mu - param.Mw * om) * \
  (param.M * param.mu + param.Mw * om);*/

  return 4. * om * (pow(param.mu, 2) * pow(param.M - param.U, 3) + \
    (-param.M * pow(param.Mw, 2) + param.U - pow(param.M, 2) * param.U + 2. * \
    pow(param.Mw, 2) * param.U + 5. * param.M * pow(param.U, 2) - 5. * pow(param.U, 3)) * \
    pow(om, 2));

}

complex disp::k2 (const complex &om, const params &param)
{
  /*return (1. + 2. * pow(param.Mw, 2)) * pow(om, 4) - pow(param.M, 2) * \
  (6. * pow(param.mu, 2) * pow(om, 2) + pow(om, 4));*/

  return pow(om, 2) * (-6. * pow(param.mu, 2) * pow(param.M - param.U, 2) + \
    (1. - pow(param.M, 2) + 2. * pow(param.Mw, 2) + 10. * param.M * param.U - \
    15. * pow(param.U, 2)) * pow(om, 2));
}

complex disp::k1 (const complex &om, const params &param)
{
  //return 2. * param.M * pow(om, 3) * (2. * pow(param.mu, 2) + pow(om, 2));

  return 4. * pow(param.mu, 2) * (param.M - param.U) * pow(om, 3) + 2. * \
    (param.M - 3. * param.U) * pow(om, 5);
}

complex disp::k0 (const complex &om, const params &param)
{
  return -pow(om, 4) * (pow(param.mu, 2) + pow(om, 2));
}

////////////////////////////////////
////// DISP_RELATION FUNCTIONS  ////
////////////////////////////////////

complex disp::F (complex om, complex k, const params &param)
{
  return (pow (om, 2) - pow (param.Mw * k, 2) - param.D * pow (k, 4)) * \
                sqrt (pow (k, 2) - pow (om - param.M * k, 2)) + param.mu * pow (om - param.M * k, 2);
}

complex disp::F (complex om, complex k, double U, const params &param)
{
  return F (om + U * k, k, param);
}

complex disp::F2 (complex om, complex k, const params &param)
{
  return pow (pow (om, 2) - pow (param.Mw * k, 2) - param.D * pow (k, 4), 2) *
        (pow (k, 2) - pow (om - param.M * k, 2)) - pow (param.mu, 2) * pow (om - param.M * k, 4);
}

complex disp::F2 (complex om, complex k, double U, const params &param)
{
  return F2 (om + U * k, k, param);
}
