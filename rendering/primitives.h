#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "../state.h"
#include <stdio.h>
#include <queue>

typedef enum{
    SPHERE,
    RECTANGLE,
    GROUND,
    CONE,
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
        int getSize();
        void Append(PrimitiveType type, int32_t colorID);
        void UpdateTransform(int index);
        void UpdateAllTransform();
        void UpdateInfo(int index);
        void UpdateAllInfo();
        void UpdatePrimitiveSize();
        void UpdateSpecified();
        void Bind();
        void Move(int index, float xTranslation, float yTranslation, float zTranslation);
        void Rotate(int index, float xRotation, float yRotation, float zRotation);
        void Scale(int index, float xScale, float yScale, float zScale);
        void Data(int index, glm::vec4 data, float bonusRotateData, float bonusScaleData, float bonusPositionData);

    private:
        GLuint primitiveCountUBO;
        GLuint possibleHitTempleteBuffer;
        GLuint possibleHitListBuffer;
        GLuint infoBuffer;
        GLuint transformBuffer;
        std::vector<PrimitiveTransform> primitiveTransforms;
        std::vector<PrimitiveInfo> primitiveInfo;
        std::vector<uint> possibleHitTemplete;
};