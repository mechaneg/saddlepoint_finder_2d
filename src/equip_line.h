/// \file equip_line.h

#ifndef EQUIP_LINE_H
#define EQUIP_LINE_H

#include "om_k.h"
#include "real_axe_intersection_state.h"

#include <vector>

class equip_line
{
private:
  unsigned int real_axe_index = 0;
  std::vector<om_k> points;
  real_axe_intersection state = real_axe_intersection::null;

public:

  equip_line (unsigned int real_axe_index_) : real_axe_index (real_axe_index_) {}
  ~equip_line () {}

  inline unsigned int get_real_axe_index () { return real_axe_index; }
  inline const std::vector<om_k>& get_points () { return points; }
  inline real_axe_intersection get_state () { return state; }

  int self_build ();

};


#endif // EQUIP_LINE_H
