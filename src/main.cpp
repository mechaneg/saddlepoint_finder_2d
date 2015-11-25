#include "common_defines.h"
#include "model_controller.h"

#include <iostream>
#include <memory>

int main (int argc, char* argv[])
{
  try
  {
    std::unique_ptr<model_controller> model_ctrl (new model_controller);

    if (model_ctrl->init_internal_structures (argc, argv) < 0)
      return -1;

    if (model_ctrl->process_model () < 0)
      return -1;

    return 0;
  }
  catch (std::exception &e)
  {
    std::cout << e.what () << "\n";
  }

  return 0;
}
