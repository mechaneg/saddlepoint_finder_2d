#ifndef BRANCH_ON_REAL_AXE_H
#define BRANCH_ON_REAL_AXE_H

/// \file branch_on_real_axe.h

#include <vector>
#include <QString>
#include "om_k.h"

class report_system;
class params;

class branch_on_real_axe
{
private:
  std::vector<om_k> points;
  double re_om_min = 0.;
  double re_om_max = 0.;
  std::vector<unsigned int> im_om_max_positions;    // indices of local maximums
  std::vector<unsigned int> im_om_min_positions;    // indices of local minimums

public:
  int calc_points (report_system *rep, const params &param);
  int calc_extremum_positions (report_system *rep);
  void calc_real_om_minmax ();

  int dump_points (report_system *rep, const std::string &file_name);
};

#endif
