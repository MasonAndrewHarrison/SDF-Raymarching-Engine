#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int main(void) {

    if (!glfwInit()) return -1;

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cuastic Gaussian Demo", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window); // what else would go in here?
    glewExperimental = GL_TRUE;
    glewInit();

    printf("%s", glGetString(GL_VERSION));

    float position[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f,
    };

    unsigned int buffer;  // what is unsigned
    glGenBuffers(1, &buffer); // is the buffer on the cpu only and the position on the gpu? // does the 1 mean
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);// still don't understand.

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
} 