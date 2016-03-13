/// \file equip_line.h

#ifndef EQUIP_LINE_H
#define EQUIP_LINE_H

#include "branch_on_real_axe.h"
#include "disp_relation.h"
#include "jenkins_traub.h"
#include "om_k.h"
#include "params.h"
#include "real_axe_intersection_state.h"

#include <vector>

class report_system;
typedef std::complex<double> complex;

enum class equip_line_direction   // while its building. depends on the sign of used om_delta
{
  positive,
  negative
};

const char* enum_to_string (const equip_line_direction eqline_dir);

class equip_line
{
private:
  om_k reference_point;
  std::vector<om_k> points;
  equip_line_direction direction = equip_line_direction::positive;
  real_axe_intersection intersection_state = real_axe_intersection::no;
  bool is_build_flag = false;

public:

  equip_line (om_k ref_point) : reference_point (ref_point) {}
  ~equip_line () {}

  inline om_k get_ref_point () const { return reference_point; }
  inline const std::vector<om_k>& get_points () const { return points; }
  inline real_axe_intersection get_intersection_state () const { return intersection_state; }
  inline equip_line_direction get_direction () const { return direction; }
  inline bool is_build () const { return is_build_flag; }

  int self_build (report_system *rep,
    const params &param,
    double om_delta,        // starting value of om_delta
    double om_delta_total,  // absolute value of Re (om_last) - Re (om_first)
    om_k_evaluator &evaluator);

  int self_build_until_real_axe_intersection (report_system *rep,
    const branch_on_real_axe &real_axe,
    const params &param,
    double om_delta,
    om_k_evaluator &evaluator);

  int dump_points (report_system *rep, const std::string &file_name) const;
};


#endif // EQUIP_LINE_H
