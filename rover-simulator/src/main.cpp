#include <iostream>
#include <vector>
#include <memory>
#include <rover/simulator/core/cmd/Command.h>
#include <rover/simulator/core/cmd/TRNCommand.h>
#include <rover/simulator/core/cmd/RCKCommand.h>

int main() {
    namespace core = rover::simulator::core;
    namespace cmd = core::cmd;

    using CommandVector = std::vector<std::unique_ptr<cmd::Command>>;
    CommandVector commands;

    auto c1 = std::make_unique<cmd::TRNCommand>(cmd::TRNCommand::Orientation::RIGHT);
    auto c2 = std::make_unique<cmd::RCKCommand>();
    commands.emplace_back(std::move(c1));
    commands.emplace_back(std::move(c2));

    for (const auto& command: commands) {
        command->run();
    }
}
