#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "camera.h"

void moveCamera(vec3 move, camera* cam){
    glm_vec3_add(cam->eye, move, cam->eye);
    glm_vec3_add(cam->center, move, cam->center);
}

void updateCamera(GLFWwindow* window, camera* cam){

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        vec3 move = {0.0f, 0.0f, -0.1f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        vec3 move = {0.0f, 0.0f, 0.1f};
        moveCamera(move, cam);
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 move = {-0.1f, 0.0f, -0.0f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 move = {0.1f, 0.0f, -0.0f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        vec3 move = {0.0f, 0.1f, -0.0f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        vec3 move = {0.0f, -0.1f, -0.0f};
        moveCamera(move, cam);
    }
}

void mouseCallback(GLFWwindow*  window, double xpos, double ypos){

    camera* cam = glfwGetWindowUserPointer(window);
    


}