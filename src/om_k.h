/// \file om_k.h

#ifndef OM_K_H
#define OM_K_H

#include <complex>
#include <vector>

class report_system;
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

int dump_vector_of_om_k (const std::vector<om_k> &points, report_system *rep, const std::string &file_name);

#endif // OM_K_H

