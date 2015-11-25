#ifndef MODEL_CONTROLLER_H
#define MODEL_CONTROLLER_H

#include "model.h"
#include "cmd_params.h"
class report_system;

class model_controller
{
private:
  report_system *rep;
  model m_model;
  cmd_params cmd_param;

public:
  model_controller ();
  ~model_controller ();

  int init_internal_structures (int argc, char *argv[]);
  int process_model ();

};

#endif // MODEL_CONTROLLER_H
