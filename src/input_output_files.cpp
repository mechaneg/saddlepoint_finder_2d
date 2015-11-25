/// \file input_output_files.cpp

#include "input_output_files.h"
#include "report_system.h"

#include <QDir>
//#include <QDebug>

int input_output_files::construct_names (report_system *rep, const cmd_params &cmd)
{
  results_folder = QString ("RESULTS");
  res_suffix     = QString (".res");
  model_suffix   = QString (".data");

  /// construct abs_model_name
  QString cur_dir (QDir::currentPath ());
  QString cmd_model_name (cmd.get_model_name ().c_str ());
  cmd_model_name.prepend ("/");

  abs_model_name = QDir::cleanPath (cur_dir + cmd_model_name);
  if (!abs_model_name.endsWith (model_suffix))
    {
      rep->print ("Error: model name should end with '.data'.\n");
      return -1;
    }

  /// construct model_name
  int suffix_ind = abs_model_name.lastIndexOf (model_suffix);
  int model_name_start_ind = abs_model_name.lastIndexOf ('/');
  model_name_start_ind++;
  if (model_name_start_ind == suffix_ind)
    {
      rep->print ("Error: model name should not be empty.\n");
      return -1;
    }
  const QChar* abs_model_name_data = abs_model_name.data ();
  for (int i = model_name_start_ind; i < suffix_ind; i++)
    model_name.push_back (abs_model_name_data[i]);

  /// construct model_path
  for (int i = 0; i < model_name_start_ind; i++)
    model_path.push_back (abs_model_name_data[i]);

  /// construct results_path
  results_path = model_path + results_folder;
  results_path.append ("/");

//  qDebug () << cur_dir << "\n"
//            << abs_model_name << "\n"
//            << model_name << "\n"
//            << model_path << "\n"
//            << results_path << "\n";

  return 0;
}
