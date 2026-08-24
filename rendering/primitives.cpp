#include "primitives.h"


Primitives::Primitives(){
    primitiveTransforms.reserve(1000);
    primitiveType.reserve(1000);
}

void Primitives::Append(PrimitivesType type, glm::vec3 position, glm::vec3 scale){

    primitiveType.push_back(type);

    PrimitiveTransform transform = {
        .position = glm::vec4(position, 0.0),
        .scale = glm::vec4(scale, 0.0)
    };
    primitiveTransforms.push_back(transform);
}

void Primitives::Bind(){

    glCreateBuffers(1, &typeBuffer);
    glNamedBufferStorage(typeBuffer, sizeof(int32_t) * primitiveType.size(), (const void *)primitiveType.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, typeBuffer);

    glCreateBuffers(1, &transformBuffer);
    glNamedBufferStorage(transformBuffer, sizeof(PrimitiveTransform) * primitiveTransforms.size(), (const void *)primitiveTransforms.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, transformBuffer);
}