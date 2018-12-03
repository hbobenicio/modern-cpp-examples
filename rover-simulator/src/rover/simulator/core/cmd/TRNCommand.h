#ifndef ROVER_SIMULATOR_CORE_TRNCOMMAND_H
#define ROVER_SIMULATOR_CORE_TRNCOMMAND_H

#include "Command.h"

namespace rover { namespace simulator { namespace core { namespace cmd {

class TRNCommand: public Command
{
public:
    enum class Orientation {
        LEFT, RIGHT
    };

private:
    Orientation orientation;
    float angle;

public:
    TRNCommand(Orientation orientation, float angle = 15.0f);
    void run() const;
};

}}}} // namespaces

#endif
