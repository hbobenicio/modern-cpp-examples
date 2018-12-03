#ifndef ROVER_SIMULATOR_CORE_CMD_RCKCOMMAND_H
#define ROVER_SIMULATOR_CORE_CMD_RCKCOMMAND_H

#include "Command.h"

namespace rover { namespace simulator { namespace core { namespace cmd {

class RCKCommand: public Command {
public:
    void run() const;
};

}}}} // namespaces

#endif
