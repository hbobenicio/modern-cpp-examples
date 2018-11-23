/**
 * modern-opengl example
 *
 * # Dependencies
 *
 * $ sudo apt install libglfw3-dev
 */
#include <iostream>

// TODO Add glad!
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <modern-opengl/common/Timer.h>

/**
 * Callback called on GLFW Error Events.
 */
static void onGLFWError(int error, const char* description) {
    std::cerr << "GLFW Error: [" << error << "] " << description << "\n";
}

/**
 * Logs the key event to the stdio.
 */
static void logKeyEvent(int key, int scancode, int action, int mods) {
    printf("KeyEvent{key: %d, scancode: %d, action: %d, mods: %d}\n", key, scancode, action, mods);
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

static const char* vertexShaderCode =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main() {\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragmentShaderCode =
"varying vec3 color;\n"
"void main() {\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll for and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ensures the correct destruction of the window and consequently the context
    glfwTerminate();
}
