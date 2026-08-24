#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

typedef enum{
    SPHERE,
    RECTANGLE,
    GROUND,
} PrimitiveType;

typedef struct{
    glm::vec4 position;
    glm::vec4 rotation;
    glm::vec4 scale;
    glm::vec4 data;
} PrimitiveTransform;

typedef struct{
    int32_t type;
    int32_t colorID;
} PrimitiveInfo;


class Primitives{
    public:
        Primitives();
        void Append(PrimitiveType type, int32_t colorID, glm::vec3 position, glm::vec3 scale);
        void Bind();
    private:
        std::vector<PrimitiveTransform> primitiveTransforms;
        std::vector<PrimitiveInfo> primitiveInfo;
        GLuint infoBuffer;
        GLuint transformBuffer;
};