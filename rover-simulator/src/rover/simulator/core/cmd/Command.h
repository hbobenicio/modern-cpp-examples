#ifndef ROVER_SIMULATOR_CORE_CMD_COMMAND_H
#define ROVER_SIMULATOR_CORE_CMD_COMMAND_H

namespace rover { namespace simulator { namespace core { namespace cmd {

class Command {
public:
    virtual void run() const = 0;
    virtual ~Command();
};

}}}} // namespaces

#endif
