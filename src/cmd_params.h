#ifndef CMD_PARAMS_H
#define CMD_PARAMS_H

#include <string>
class report_system;

class cmd_params
{
private:
  std::string model_name;

public:
  int parse_cmd (report_system *rep, int argc, char *argv[]);
  const std::string& get_model_name () const;
};

#endif // CMD_PARAMS_H
