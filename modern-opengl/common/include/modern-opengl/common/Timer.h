#ifndef MODERN_OPENGL_COMMMON_TIMER_H
#define MODERN_OPENGL_COMMMON_TIMER_H

#include <functional>

namespace ModernOpenGL { namespace Common {

class Timer {
private:
    double previousTime;
    int frameCount;
    float limit;

public:
    Timer(float limit);

    void tick(const std::function<void(int)>& callback);
};

}}

#endif
