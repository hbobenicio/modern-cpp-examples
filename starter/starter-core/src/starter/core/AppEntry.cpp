#include <starter/core/AppEntry.h>

namespace starter { namespace core {

    AppEntry::AppEntry(const std::string& name, const std::string& cmd, const std::string& iconPath)
        : name(name), cmd(cmd), iconPath(iconPath)
    {
    }

    std::vector<AppEntry> AppEntry::mockEntries() {
        std::vector<AppEntry> entries;
        entries.reserve(3);

        const AppEntry firefoxQuantum{"Firefox Quantum", "/home/01372329340/bin/firefox", "firefox-quantum.png"};
        const AppEntry firefoxSerpro{"Firefox Serpro", "firefox", "firefox.png"};
        const AppEntry gnomeTerminal{"Gnome Terminal", "gnome-terminal", "terminal.png"};

        entries.push_back(firefoxQuantum);
        entries.push_back(firefoxSerpro);
        entries.push_back(gnomeTerminal);

        return entries;
    }

}}
