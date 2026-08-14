#pragma once
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



static const float SCREEN_VERTICES[] = {
1.0f,  1.0f,
1.0f, -1.0f,
-1.0f, -1.0f,
-1.0f,  1.0f,
};

static const unsigned int SCREEN_INDICES[] = {
0, 1, 2,
2, 3, 0,
};

class Screen {
public:
    Screen();
    ~Screen();
    void draw();

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
};

