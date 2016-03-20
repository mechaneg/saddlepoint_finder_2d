#ifndef INPUT_OUTPUT_FILES_H
#define INPUT_OUTPUT_FILES_H

#include "common_defines.h"

#include <QString>

struct cmd_params;
class report_system;

class input_output_files
{
public:

  QString model_name;
  QString abs_model_name;

  QString model_path;
  QString results_path;

  QString real_axe_name;

  static const QString results_folder;
  static const QString res_suffix;
  static const QString model_suffix;
  static const QString real_axe_suffix;

private:
  int abs_model_name_init (report_system *rep, const cmd_params &cmd);
  int model_name_and_path_init (report_system *rep);
  void results_path_init ();
  void real_axe_name_init ();

public:
  int construct_names (report_system *rep, const cmd_params &cmd);
  int create_result_dir (report_system *rep);
};

#endif // INPUT_OUTPUT_FILES_H

