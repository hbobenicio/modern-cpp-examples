#include "TRNCommand.h"
#include <iostream>

namespace rover { namespace simulator { namespace core { namespace cmd {

TRNCommand::TRNCommand(Orientation orientation, float angle)
    : orientation(orientation), angle(angle)
{
}

void TRNCommand::run() const {
    std::string formattedOrientation = this->orientation == Orientation::LEFT ? "Left" : "Right";

    std::cout << "[TRN] Turning " << this->angle << " degrees to the " << formattedOrientation << '\n';
}

}}}} // namespaces
