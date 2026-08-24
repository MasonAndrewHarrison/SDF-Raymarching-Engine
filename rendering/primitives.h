#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

typedef enum{
    SPHERE,
    RECTANGLE,
} PrimitivesType;

typedef struct{
    glm::vec4 position;
    glm::vec4 rotation;
    glm::vec4 scale;
    glm::vec4 data;
} PrimitiveTransform;


class Primitives{
    public:
        Primitives();
        void Append(PrimitivesType type, glm::vec3 position);
        void Bind();
    private:
        std::vector<PrimitiveTransform> primitiveTransforms;
        std::vector<int32_t> primitiveType;
        GLuint typeBuffer;
        GLuint transformBuffer;
};