#ifndef ROOTF_H
#define ROOTF_H

/// \file jenkins_traub.h

/*
 *     INPUT:
 *     opr - double precision vector of real coefficients in order of
 *          decreasing powers.
 *     opi - double precision vector of imaginary coefficients in order of
 *          decreasing powers.
 *     degree - integer degree of polynomial
 *
 *     OUTPUT:
 *     zeror,zeroi - output double precision vectors of the
 *          real and imaginary parts of the zeros.
 *            to be consistent with rpoly.cpp the zeros is inthe index
 *            [0..max_degree-1]
 *
 *     RETURN:
 *     returnval:   -1 if leading coefficient is zero, otherwise
 *          number of roots found.
*/


#define MAXDEGREE 100
#define MDP1 MAXDEGREE+1

class jenkins_traub
{
private:
  unsigned max_deg = 0;

  double sr = 0.;
  double si = 0.;
  double tr = 0.;
  double ti = 0.;
  double pvr = 0.;
  double pvi = 0.;
  double are = 0.;
  double mre = 0.;
  double eta = 0.;
  double infin = 0.;

  int nn = 0;
  double *pr  = nullptr;
  double *pi  = nullptr;
  double *hr  = nullptr;
  double *hi  = nullptr;
  double *qpr = nullptr;
  double *qpi = nullptr;
  double *qhr = nullptr;
  double *qhi = nullptr;
  double *shr = nullptr;
  double *shi = nullptr;

private:
  void noshft (int l1);
  void fxshft (int l2, double *zr, double *zi, int *conv);
  void vrshft (int l3, double *zr, double *zi, int *conv);
  void calct  (int *bol);
  void nexth  (int bol);

  void polyev (int nn, double sr, double si, const double *pr,
               const double *pi, double *qr, double *qi, double *pvr, double *pvi);

  double errev (int nn, const double *qr, const double *qi, double ms,
                double mp, double are, double mre);

  void cauchy  (int nn, double *pt, double *q, double *fn_val);
  void cdivid  (double ar, double ai, double br, double bi, double *cr, double *ci);
  double scale (int nn, const double *pt, double eta, double infin, double smalno, double base);
  double cmod  (double r, double i);
  void mcon (double *eta, double *infiny, double *smalno, double *base);

public:

  ~jenkins_traub ();

  int init (unsigned max_degree);
  int check ();
  int cpoly (const double *opr, const double *opi, unsigned actual_deg, double *zeror, double *zeroi);
};

#endif /* ROOTF_H */
