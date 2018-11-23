#include <modern-opengl/common/Timer.h>
#include <GLFW/glfw3.h>

namespace ModernOpenGL { namespace Common {

Timer::Timer(float limit)
    : previousTime(glfwGetTime()), frameCount(0), limit(limit)
{
}

void Timer::tick(const std::function<void(int)>& callback) {
    // Measure speed
    double currentTime = glfwGetTime();
    this->frameCount++;

    // If a second has passed.
    if (currentTime - previousTime >= this->limit) {
        callback(this->frameCount);

        frameCount = 0;
        previousTime = currentTime;
    }
}

}} // namespaces
