#ifndef DISP_RELATION_H
#define DISP_RELATION_H

/// \file disp_relation.h

#include <vector>
#include <complex>

typedef std::complex<double> complex;
class params;
class jenkins_traub;

class disp_relation
{
private:
  unsigned max_om_poly_deg = 0;
  unsigned  max_k_poly_deg = 0;

  complex *om_coeff   = nullptr;
  double *om_coeff_re = nullptr;
  double *om_coeff_im = nullptr;
  double *om_zero_re  = nullptr;
  double *om_zero_im  = nullptr;

  complex *k_coeff   = nullptr;
  double *k_coeff_re = nullptr;
  double *k_coeff_im = nullptr;
  double *k_zero_re  = nullptr;
  double *k_zero_im  = nullptr;

public:
  disp_relation () = default;
  ~disp_relation ();

  int init (unsigned maximum_om_poly_deg, unsigned maximum_k_poly_deg);
  int check ();

  disp_relation (const disp_relation &origin) = default;
  disp_relation& operator= (const disp_relation &origin) = default;

  static complex om6 (const complex &k, const params &param);
  static complex om5 (const complex &k, const params &param);
  static complex om4 (const complex &k, const params &param);
  static complex om3 (const complex &k, const params &param);
  static complex om2 (const complex &k, const params &param);
  static complex om1 (const complex &k, const params &param);
  static complex om0 (const complex &k, const params &param);

  static complex k10 (const complex &om, const params &param);
  static complex k9 (const complex &om, const params &param);
  static complex k8 (const complex &om, const params &param);
  static complex k7 (const complex &om, const params &param);
  static complex k6 (const complex &om, const params &param);
  static complex k5 (const complex &om, const params &param);
  static complex k4 (const complex &om, const params &param);
  static complex k3 (const complex &om, const params &param);
  static complex k2 (const complex &om, const params &param);
  static complex k1 (const complex &om, const params &param);
  static complex k0 (const complex &om, const params &param);

  static complex F  (const complex &om, const complex &k, const params &param);
  static complex F  (const complex &om, const complex &k, double U, const params &param);
  static complex F2 (const complex &om, const complex &k, const params &param);
  static complex F2 (const complex &om, const complex &k, double U, const params &param);

  int calc_k  (std::vector<complex> &k, const complex &om, const params &param, jenkins_traub &jt_k);
  int calc_om (std::vector<complex> &om, const complex &k, const params &param, jenkins_traub &jt_om);
};

#endif /* DISP_RELATION_H */
