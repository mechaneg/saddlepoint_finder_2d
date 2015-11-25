/// \file disp_rel.cpp

#include "disp_rel.h"
#include "common_defines.h"
#include "params.h"

#include <cmath>

void disp::calc_om(std::valarray<complex<double>>& om, complex<double>k, const param_t& param)
{
	static complex<double>* om_coeff = new complex<double>[OM_POLY_DEG + 1];
	static double* om_coeff_re = new double[OM_POLY_DEG + 1];
	static double* om_coeff_im = new double[OM_POLY_DEG + 1]; 
	static double* om_zero_re  = new double[OM_POLY_DEG];
	static double* om_zero_im  = new double[OM_POLY_DEG];

	int i, j;

	if(om.size() != OM_POLY_DEG)
	{
		std::cout << "size of input valarray om != om_poly_deg\n";
		std::cout << "k = " << k << ". params = (" << param.Mw << " " << param.M << " "\
			<< param.D << " " << param.mu << ")\n";
		std::cin.get();
		std::abort();
	}

//////////////

	om_coeff[6] = om0(k, param);		//inverse order in rootfinder
	om_coeff[5] = om1(k, param);
	om_coeff[4] = om2(k, param);
	om_coeff[3] = om3(k, param);
	om_coeff[2] = om4(k, param);
	om_coeff[1] = om5(k, param);
	om_coeff[0] = om6(k, param);

	for(i = 0; i < OM_POLY_DEG + 1; i++)
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

complex<double> disp::om4 (const complex &k, const params &param)
{
	return 2. * param.D * pow(k,4) - pow(param.mu,2) + pow(k,2) * (2. * pow(param.Mw,2) - pow(param.M,2) + 1.);
}

complex<double> disp::om3 (const complex &k, const params &param)
{
	return -4. * k * param.M * (param.D * pow(k,4) - pow(param.mu,2) + pow(k,2) * pow(param.Mw,2) );
}

complex<double> disp::om2 (const complex &k, const params &param)
{
	return -pow(k,2) * (pow(param.D,2) * pow(k,6) + 6. * pow(param.M,2) * pow(param.mu,2) - 2. * pow(k,2) * (pow(param.M,2) - 1.) * \
		pow(param.Mw,2) + pow(k,2) * pow(param.Mw,4) + 2. * param.D * pow(k,4) * (1. - pow(param.M,2) + pow(param.Mw,2)));
}

complex<double> disp::om1 (const complex &k, const params &param)
{
	return 2. * pow(k,3) * param.M * (2. * pow(param.M,2) * pow(param.mu,2) + pow(k,2) * pow(param.D * pow(k,2) + pow(param.Mw,2),2));
}

complex<double> disp::om0 (const complex &k, const params &param)
{
	return -pow(k,4) * (pow(param.D,2) * pow(k,6) * (pow(param.M,2) - 1.) + pow(param.M,4) * pow(param.mu,2) + \
		2. * param.D * pow(k,4) * (pow(param.M,2) - 1.) * pow(param.Mw,2) + pow(k,2) * (pow(param.M,2) - 1.) * pow(param.Mw,4));
}

////////////////////////////////////
////// DISP_RELATION FUNCTIONS  ////
////////////////////////////////////

complex<double> disp::F (complex om, complex k, const params &param)
{
	return (pow(om, 2) - pow(param.Mw * k, 2) - param.D * pow(k, 4)) * \
		sqrt(pow(k, 2) - pow(om - param.M * k, 2)) + param.mu * pow(om - param.M * k, 2);
}

complex<double> disp::F(complex<double> om, complex<double> k, double U, const param_t& param)
{
	return F(om + U * k, k, param); 
}

complex<double> disp::F2(complex<double> om, complex<double> k, const param_t& param)
{
	return pow(pow(om, 2) - pow(param.Mw * k, 2) - param.D * pow(k, 4), 2) * \
		(pow(k, 2) - pow(om - param.M * k, 2)) - pow(param.mu, 2) * pow(om - param.M * k, 4);
}

complex<double> disp::F2(complex<double> om, complex<double> k, double U, const param_t& param)
{
	return F2(om + U * k, k, param); 
}
