/// \file om_k.h

#ifndef OM_K_H
#define OM_K_H

#include <complex>

typedef std::complex<double> complex;

class om_k
{
public:
  complex om = 0.;
  complex k  = 0.;

  om_k () = default;
  om_k (complex _om, complex _k) :
    om (_om), k (_k) {}
};

#endif // OM_K_H

