/**
 * Instalando as dependências:
 * sudo apt install libglfw3-dev
 */

#include <iostream>
#include <functional>

// Do **not** include the OpenGL header yourself, as GLFW does this for you in a platform-independent way
// If you do need to include such headers, include them **before** the GLFW header and it will detect this
// #include <GL/gl.h>

#include <GLFW/glfw3.h>

namespace Utils {
    class Timer {
    private:
        double previousTime = glfwGetTime();
        int frameCount = 0;
        float limit;

    public:
        Timer(float limit) : limit(limit)
        {
        }

        void tick(const std::function<void(int)>& callback) {
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
    };
}

static void logKeyEvent(int key, int scancode, int action, int mods) {
    printf("KeyEvent{key: %d, scancode: %d, action: %d, mods: %d}\n", key, scancode, action, mods);
}

/**
 * Callback called on GLFW Error Events.
 */
static void onGLFWError(int error, const char* description) {
    std::cerr << "GLFW Error: [" << error << "] " << description << "\n";
}

/**
 * Callback called for GLFW Key Events.
 */
static void onGLFWKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    logKeyEvent(key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // Callback functions must be set, so GLFW knows to call them.
    // The function to set the error callback is one of the few GLFW functions that may be called before
    // initialization, which lets you be notified of errors both during and after initialization.
    glfwSetErrorCallback(onGLFWError);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error while initializing GLFW\n";
        return EXIT_FAILURE;
    }

    // By default, the OpenGL context GLFW creates may have any version.
    // You can require a minimum OpenGL version by setting the
    // GLFW_CONTEXT_VERSION_MAJOR and GLFW_CONTEXT_VERSION_MINOR hints before creation.
    // If the required minimum version is not supported on the machine, context (and window) creation fails.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "hello-world", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error while creating the glfw window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Registers the key event callback
    glfwSetKeyCallback(window, onGLFWKey);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    Utils::Timer fpsTimer{1.0};

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        fpsTimer.tick([](int fps){
            // Display the frame count here any way you want.
            std::cout << "FPS: " << fps << "\n";
        });

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll for and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ensures the correct destruction of the window and consequently the context
    glfwTerminate();
}
