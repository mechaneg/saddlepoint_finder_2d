#ifndef DISP_REL_H
#define DISP_REL_H

/// \file disp_rel.h

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

  complex k10 (const complex &om, const params &param);
  complex k9 (const complex &om, const params &param);
  complex k8 (const complex &om, const params &param);
  complex k7 (const complex &om, const params &param);
  complex k6 (const complex &om, const params &param);
  complex k5 (const complex &om, const params &param);
  complex k4 (const complex &om, const params &param);
  complex k3 (const complex &om, const params &param);
  complex k2 (const complex &om, const params &param);
  complex k1 (const complex &om, const params &param);
  complex k0 (const complex &om, const params &param);

  void calc_om (std::valarray<complex<double>>& om, const complex &k, const params &param);
  //void calc_k(std::valarray<complex<double>> k, complex<double> om, const param_t& param);

  complex F (const complex &om, const complex &k, const params &param);
  complex F (const complex &om, const complex &k, double U, const params &param);
  complex F2 (const complex &om, const complex &k, const params &param);
  complex F2 (const complex &om, const complex &k, double U, const params &param);
}

#endif /* DISP_REL_H */
