#include "model_controller.h"
#include "report_system.h"
#include "common_defines.h"

model_controller::model_controller ()
{
  rep = new report_system;
}

model_controller::~model_controller ()
{
  FREE (rep);
}

int model_controller::init_internal_structures (int argc, char *argv[])
{
  if (cmd_param.parse_cmd (rep, argc, argv) < 0)
    return -1;

  return 0;
}

int model_controller::process_model ()
{
  /// model initializing
  if (m_model.init_model (rep, cmd_param) < 0)
    {
      rep->print ("Error: cannot initialize model.\n");
      return -1;
    }
  else
    rep->print ("Info: model was successfully initialized.\n");

  return 0;
}