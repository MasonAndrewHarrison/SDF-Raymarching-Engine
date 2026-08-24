#include "primitives.h"


Primitives::Primitives(){
    primitiveTransforms.reserve(1000);
    primitiveInfo.reserve(1000);
}

void Primitives::Append(PrimitiveType type, int32_t colorID, glm::vec3 position, glm::vec3 scale){

    primitiveInfo.push_back({type, colorID});

    PrimitiveTransform transform = {
        .position = glm::vec4(position, 0.0),
        .scale = glm::vec4(scale, 0.0)
    };
    primitiveTransforms.push_back(transform);
}

void Primitives::Bind(){

    glCreateBuffers(1, &infoBuffer);
    glNamedBufferStorage(infoBuffer, sizeof(int32_t) * 2 * primitiveInfo.size(), (const void *)primitiveInfo.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, infoBuffer);

    glCreateBuffers(1, &transformBuffer);
    glNamedBufferStorage(transformBuffer, sizeof(PrimitiveTransform) * primitiveTransforms.size(), (const void *)primitiveTransforms.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, transformBuffer);
}