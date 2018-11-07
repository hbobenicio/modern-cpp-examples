/**
 * Instalando as dependências:
 * sudo apt install libglfw3-dev
 */
#include <iostream>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

int main() {

    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "Error while initing glfw\n";
        return EXIT_FAILURE;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cerr << "Error while creating the glfw window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    // ensures the correct destruction of the window and consequently the context
    glfwTerminate();
}
