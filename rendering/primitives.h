#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "../state.h"
#include <stdio.h>
#include <queue>
#include <algorithm>


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
    float boundingDistance;
} PrimitiveInfo;


class Primitives{

    public:
        Primitives();
        int getSize();
        void append(PrimitiveType type, int32_t colorID);
        void updateTransform(int index);
        void updateAllTransform();
        void updateInfo(int index);
        void updateAllInfo();
        void updatePrimitiveSize();
        void updateSpecified();
        void bind();
        void move(int index, float xTranslation, float yTranslation, float zTranslation);
        void rotate(int index, float xRotation, float yRotation, float zRotation);
        void scale(int index, float xScale, float yScale, float zScale);
        void data(int index, glm::vec4 data, float bonusRotateData, float bonusScaleData, float bonusPositionData);

    private:
        float getShapeUnitDistance(PrimitiveType type);
        GLuint primitiveCountUBO;
        GLuint possibleHitTempleteBuffer;
        GLuint possibleHitListBuffer;
        GLuint infoBuffer;
        GLuint transformBuffer;
        std::vector<PrimitiveTransform> primitiveTransforms;
        std::vector<PrimitiveInfo> primitiveInfo;
        std::vector<uint> possibleHitTemplete;
};