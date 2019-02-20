#ifndef STARTER_CORE_FS_PATHS_H
#define STARTER_CORE_FS_PATHS_H

namespace starter { namespace core { namespace fs {

    #ifdef _WIN32
        const char sep = '\\';
    #else
        const char sep = '/';
    #endif
}}}

#endif
