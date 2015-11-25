/// \file disp_rel.h

#ifndef DISP_REL_H
#define DISP_REL_H

typedef std::complex<double> complex;

class params;

namespace disp
{
  complex om6 (const complex &k, const params &param);
  complex om5 (const complex &k, const params &param);
  complex om4 (const complex &k, const params &param);
  complex om3 (const complex &k, const params &param);
  complex om2 (const complex &k, const params &param);
  complex om1 (const complex &k, const params &param);
  complex om0 (const complex &k, const params &param);

  /*double k10(double om, double Mw, double M, double D, double mu);
  double k9(double om, double Mw, double M, double D, double mu);
  double k8(double om, double Mw, double M, double D, double mu);
  double k7(double om, double Mw, double M, double D, double mu);
  double k6(double om, double Mw, double M, double D, double mu);
  double k5(double om, double Mw, double M, double D, double mu);
  double k4(double om, double Mw, double M, double D, double mu);
  double k3(double om, double Mw, double M, double D, double mu);
  double k2(double om, double Mw, double M, double D, double mu);
  double k1(double om, double Mw, double M, double D, double mu);
  double k0(double om, double Mw, double M, double D, double mu);*/

  /// write array of omega zeros in om

  void calc_om (std::valarray<complex<double>>& om, complex<double> k, const param_t& param);
  //void calc_k(std::valarray<complex<double>> k, complex<double> om, const param_t& param);

  complex F (const complex &om, const complex &k, const params &param);
  complex F (const complex &om, const complex &k, double U, const params &param);
  complex F2 (const complex &om, const complex &k, const params &param);
  complex F2 (const complex &om, const complex &k, double U, const params &param);
}


#endif /* DISP_REL_H */
