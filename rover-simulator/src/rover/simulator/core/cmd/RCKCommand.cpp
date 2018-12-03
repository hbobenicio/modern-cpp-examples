#include "RCKCommand.h"
#include <iostream>

namespace rover { namespace simulator { namespace core { namespace cmd {

void RCKCommand::run() const {
    std::cout << "[RCK] Collecting rock sample for analysis...\n";
}

}}}} // namespaces
