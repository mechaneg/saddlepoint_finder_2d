/// \file input_output_files.cpp

#include "input_output_files.h"
#include "report_system.h"
#include "file_system.h"

#include <QDir>
//#include <QDebug>

const QString input_output_files::results_folder = "RESULTS";
const QString input_output_files::res_suffix     = ".res";
const QString input_output_files::model_suffix   = ".data";

int input_output_files::abs_model_name_init (report_system *rep, const cmd_params &cmd)
{
  QString cmd_model_name (cmd.get_model_name ().c_str ());

  if (is_abs_filename (cmd_model_name.toStdString ().c_str ()))
    abs_model_name = std::move (cmd_model_name);
  else
    {
      QString cur_dir = QDir::currentPath ();
      abs_model_name  = QDir::cleanPath (cur_dir + "/" + cmd_model_name);
    }

  if (!abs_model_name.endsWith (model_suffix))
    {
      rep->print ("Error: model name should end with '.data'.\n");
      return -1;
    }
  return 0;
}

int input_output_files::model_name_and_path_init (report_system *rep)
{
  int suffix_ind = abs_model_name.lastIndexOf (model_suffix);
  int model_name_start_ind = abs_model_name.lastIndexOf ('/');
  model_name_start_ind++;
  if (model_name_start_ind == suffix_ind)
    {
      rep->print ("Error: model name should not be empty.\n");
      return -1;
    }
  const QChar* abs_model_name_data = abs_model_name.data ();

  for (int i = 0; i < model_name_start_ind; i++)
    model_path.push_back (abs_model_name_data[i]);

  for (int i = model_name_start_ind; i < suffix_ind; i++)
    model_name.push_back (abs_model_name_data[i]);

  return 0;
}

void input_output_files::results_path_init ()
{
  results_path = model_path + results_folder;
  results_path.append ("/");
}

int input_output_files::construct_names (report_system *rep, const cmd_params &cmd)
{
  if (abs_model_name_init (rep, cmd) < 0)
    return -1;

  if (model_name_and_path_init (rep) < 0)
    return -2;

  results_path_init ();

  return 0;
}
