#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"

#define WIDTH               800
#define HEIGHT              600
#define VSYNC_INTERVAL      1

static void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void glCheckError(const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        switch(error) {
            case GL_INVALID_ENUM:
                printf("[%s:%d] GL_INVALID_ENUM\n", file, line);
                break;
            case GL_INVALID_VALUE:
                printf("[%s:%d] GL_INVALID_VALUE\n", file, line);
                break;
            case GL_INVALID_OPERATION:
                printf("[%s:%d] GL_INVALID_OPERATION\n", file, line);
                break;
            case GL_OUT_OF_MEMORY:
                printf("[%s:%d] GL_OUT_OF_MEMORY\n", file, line);
                break;
            default:
                printf("[%s:%d] Error: %u\n", file, line, error);
                break;
        }
    }
}

#define GL_CALL(x) glClearError(); x; glCheckError(__FILE__, __LINE__);


int main(void) {

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Cuastic Gaussian Demo", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window); 
    glfwSwapInterval(VSYNC_INTERVAL);
    glewExperimental = GL_TRUE;
    glewInit();

    printf("%s\n", glGetString(GL_VERSION));

    float position[] = {
        -0.8f, -0.8f,
        0.8f, -0.8f,
        0.8f, 0.8f,
        -0.8f, 0.8f,

    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,

    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;  
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), position, GL_STATIC_DRAW); 

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);

    unsigned int ibo;
    glGenBuffers(1, &ibo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); 


    unsigned int shader = createShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    glUseProgram(shader);

    int timeLoc = glGetUniformLocation(shader, "uTime");
    int resolutionLoc = glGetUniformLocation(shader, "uResolution");
    int stretchLoc = glGetUniformLocation(shader, "uStretch");
    int angleLoc = glGetUniformLocation(shader, "uAngle");

    float startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime() - startTime;
        glUniform1f(timeLoc, currentTime);
        glUniform2f(resolutionLoc, WIDTH, HEIGHT);
        glUniform2f(stretchLoc, 3.0f, 7.0f);
        glUniform1f(angleLoc, currentTime);

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
} 