#include "cmd_params.h"
#include "report_system.h"
#include "common_defines.h"

#include <regex>

enum CMD : int
{
  PROG_NAME  = 0,
  MODEL_NAME = 1
};

int cmd_params::parse_cmd (report_system *rep, int argc, char *argv[])
{
  if (argc < 2)
    {
      rep->print ("Error: name of model is not specified.\n");
      return -1;
    }

  for (int i = 0; i < argc; i++)
    {
      switch (i)
        {
          case CMD::PROG_NAME:
            prog_name = std::string (argv[i]);
            break;
          case CMD::MODEL_NAME:
            model_name = std::string (argv[i]);
            break;
          default:
            break;
        }
    }

  return 0;
}

const std::string& cmd_params::get_model_name () const
{
  return model_name;
}

const std::string& cmd_params::get_prog_name () const
{
  return prog_name;
}




