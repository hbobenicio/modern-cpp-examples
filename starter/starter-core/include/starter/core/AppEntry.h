#ifndef STARTER_CORE_APP_ENTRY_H
#define STARTER_CORE_APP_ENTRY_H

#include <string>
#include <vector>

namespace starter { namespace core {

    struct AppEntry {
        std::string name;
        std::string cmd;
        std::string iconPath;

        AppEntry(const std::string& name, const std::string& cmd, const std::string& iconPath);

        static std::vector<AppEntry> mockEntries();
    };

}}

#endif
