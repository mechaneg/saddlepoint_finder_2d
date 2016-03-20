/// \file input_output_files.cpp

#include "cmdline.h"
#include "file_system.h"
#include "input_output_files.h"
#include "report_system.h"

#include <QDir>

const QString input_output_files::results_folder  = "RESULTS";
const QString input_output_files::res_suffix      = ".res";
const QString input_output_files::model_suffix    = ".data";
const QString input_output_files::real_axe_suffix = "real_axe";

int input_output_files::abs_model_name_init (report_system *rep, const cmd_params &cmd)
{
  QString cmd_model_name (cmd.filename_arg);

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

void input_output_files::real_axe_name_init ()
{
  real_axe_name = results_path + model_name + "_" +
      real_axe_suffix + res_suffix;
}

int input_output_files::construct_names (report_system *rep, const cmd_params &cmd)
{
  if (abs_model_name_init (rep, cmd) < 0)
    return -1;

  if (model_name_and_path_init (rep) < 0)
    return -2;

  results_path_init ();
  real_axe_name_init ();

  return 0;
}

int input_output_files::create_result_dir (report_system *rep)
{
  QDir results_dir (results_path);
  if (!results_dir.exists ())
    {
      if (!results_dir.mkpath ("."))
        {
          rep->print ("Error: cannot create result dir for the model.\n");
          return -1;
        }
    }
  return 0;
}
