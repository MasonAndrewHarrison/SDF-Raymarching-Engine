#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "glError.h"
#include <cglm/cglm.h>

#define WIDTH               800
#define HEIGHT              900
#define VSYNC_INTERVAL      1



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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    mat4 proj  = GLM_MAT4_IDENTITY_INIT;
    mat4 view  = GLM_MAT4_IDENTITY_INIT;
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 pv;
    mat4 mvp;

    glm_perspective(glm_rad(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f, proj);
    glm_translate(view, (vec3){0.0f, 0.0f, 0.0f});

    vec3 eye    = {0.0f, 0.0f,  3.0f};
    vec3 center = {0.0f, 0.0f,  0.0f};
    vec3 up     = {0.0f, 1.0f,  0.0f};

    int timeLoc = glGetUniformLocation(shader, "uTime");
    int resolutionLoc = glGetUniformLocation(shader, "uResolution");
    int stretchLoc = glGetUniformLocation(shader, "uStretch");
    int angleLoc = glGetUniformLocation(shader, "uAngle");
    int MVPLoc = glGetUniformLocation(shader, "uMVP");

    float startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            printf("W pressed\n");
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            printf("S pressed\n");
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            printf("A pressed\n");
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            printf("D pressed\n");

        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime() - startTime;
        glUniform1f(timeLoc, currentTime);
        glUniform2f(resolutionLoc, WIDTH, HEIGHT);
        glUniform2f(stretchLoc, 3.0f, 7.0f);
        glUniform1f(angleLoc, currentTime);

        glm_lookat(eye, center, up, view);
        glm_mat4_mul(proj,  view,  pv);
        glm_mat4_mul(pv,    model, mvp);
        glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, (float*)mvp);

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
} 