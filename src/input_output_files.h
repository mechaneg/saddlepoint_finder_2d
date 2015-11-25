#ifndef INPUT_OUTPUT_FILES_H
#define INPUT_OUTPUT_FILES_H

#include "cmd_params.h"
#include "common_defines.h"

#include <QString>

class cmd_params;
class report_system;

class input_output_files
{
public:

  QString model_name;
  QString abs_model_name;
  QString model_path;
  QString results_path;

  QString results_folder;
  QString res_suffix;
  QString model_suffix;

  int construct_names (report_system *rep, const cmd_params &cmd);
};

#endif // INPUT_OUTPUT_FILES_H

