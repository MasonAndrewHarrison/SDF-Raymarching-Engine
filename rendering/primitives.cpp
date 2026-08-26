#include "primitives.h"


Primitives::Primitives(){
    primitiveTransforms.reserve(1000);
    primitiveInfo.reserve(1000);
}

void Primitives::Append(PrimitiveType type, int32_t colorID){

    primitiveInfo.push_back({type, colorID}); 

    PrimitiveTransform transform = {
        .position = glm::vec4(0.0, 0.0, 0.0, 0.0),
        .rotation = glm::vec4(0.0, 0.0, 0.0, 0.0),
        .scale = glm::vec4(1.0, 1.0, 1.0, 0.0),
        .data = glm::vec4(0.0, 0.0, 0.0, 0.0),
    };
    primitiveTransforms.emplace_back(transform);
    state.needUpdate.primitiveCount = true;
}

void Primitives::Bind(){

    glCreateBuffers(1, &infoBuffer);
    glNamedBufferStorage(infoBuffer, sizeof(int32_t) * 2 * primitiveInfo.size(), (const void *)primitiveInfo.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, infoBuffer);

    glCreateBuffers(1, &transformBuffer);
    glNamedBufferStorage(transformBuffer, sizeof(PrimitiveTransform) * primitiveTransforms.size(), (const void *)primitiveTransforms.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, transformBuffer);
}

void Primitives::UpdatePrimitiveCount(){
    Bind();
}

void Primitives::UpdateTransform(int index){

    GLintptr offset = index * sizeof(PrimitiveTransform);
    GLsizeiptr size = sizeof(PrimitiveTransform);

    glNamedBufferSubData(transformBuffer, offset, size, (const void*)&primitiveTransforms.at(index));
}

void Primitives::UpdateInfo(int index){
    GLintptr offset = index * sizeof(PrimitiveInfo);
    GLsizeiptr size = sizeof(PrimitiveInfo);

    glNamedBufferSubData(infoBuffer, offset, size, (const void*)&primitiveInfo.at(index));
}

void Primitives::UpdateAllInfo(){
    
    glNamedBufferSubData(infoBuffer, 0, sizeof(PrimitiveInfo)*getSize(), (const void*)primitiveInfo.data());
}

void Primitives::UpdateAllTransform(){

    glNamedBufferSubData(transformBuffer, 0, sizeof(PrimitiveTransform)*getSize(), (const void*)primitiveTransforms.data());
}

void Primitives::Move(int index, float xTranslation, float yTranslation, float zTranslation){

    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.position[0] = xTranslation;
    shape.position[1] = yTranslation;
    shape.position[2] = zTranslation;
}

void Primitives::Rotate(int index, float xRotation, float yRotation, float zRotation){

    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.rotation[0] = xRotation;
    shape.rotation[1] = yRotation;
    shape.rotation[2] = zRotation;
}

void Primitives::Scale(int index, float xScale, float yScale, float zScale){

    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.scale[0] = xScale;
    shape.scale[1] = yScale;
    shape.scale[2] = zScale;
}

int Primitives::getSize(){
    return primitiveInfo.size();
}

/*
 * The bonus____Data is using leftover data in the PrimitiveTransform struct that is 
 * there for padding reasons.
 */
void Primitives::Data(int index, glm::vec4 data, float bonusRotateData, float bonusScaleData, float bonusPositionData){

    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.data = data;
    shape.position[3] = bonusPositionData;
    shape.scale[3] = bonusScaleData;
    shape.rotation[3] = bonusRotateData;
}