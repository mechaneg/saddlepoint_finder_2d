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
  if (cmdline_parser (argc, argv, &cmd_param) != 0)
    return -1;

  return 0;
}

int model_controller::process_model ()
{
  ////////////////////////
  /// model initializing
  ////////////////////////
  if (m_model.init_model (rep, cmd_param) < 0)
    {
      rep->print ("Error: cannot initialize model.\n");
      return -1;
    }
  rep->print ("Info: model was successfully initialized.\n");

  ///////////////////////////////////
  /// calculating branch on real axe
  ///////////////////////////////////
  if (m_model.calc_branch_on_real_axe (rep) < 0)
    {
      rep->print ("Error: cannot calculate branch info on real axe.\n");
      return -1;
    }
  rep->print ("Info: branch info on real axe was successfully calculated.\n");

  ///////////////////////////////////////////
  /// calculating equip_lines or asymptotics
  ///////////////////////////////////////////
  if (m_model.get_calc_mode () == equip_lines_mode::global_picture)
    {
      /// calculate all equipotential lines
      rep->print ("Info: start building equipotential lines, outgoing from the real axe.\n");
      if (m_model.calc_equip_lines (rep) < 0)
        {
          rep->print ("Error: cannot calculate equipotential lines, outgoing from the real axe.\n");
          return -1;
        }
      rep->print ("Info: equipotential lines, outgoing from the real axe, were successfully calculated.\n");
    }
  else if (m_model.get_calc_mode () == equip_lines_mode::asymptotics)
    {
      rep->print ("Info: 'asymptotics' mode is not supported yet.\n");
      return 0;
    }

  return 0;
}
