#include "primitives.h"


Primitives::Primitives(){
    primitiveTransforms.reserve(1000);
    primitiveInfo.reserve(1000);
    possibleHitTemplete.resize(1, ~0u);

    glCreateBuffers(1, &primitiveCountUBO);
    glNamedBufferStorage(primitiveCountUBO, sizeof(int), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, primitiveCountUBO);
}

float Primitives::getShapeUnitDistance(PrimitiveType type){

    switch (type) {
        case SPHERE: return 1;
        case RECTANGLE: return std::sqrt(3.0);
        case GROUND: return std::numeric_limits<float>::infinity();
        case CONE: return std::sqrt(2.0);
        default: return 1;
    }
}

void Primitives::append(PrimitiveType type, int32_t colorID){

    if (possibleHitTemplete.size() < (primitiveInfo.size() + 31) / 32){
        possibleHitTemplete.push_back(~0u);
    }

    primitiveInfo.push_back({type, colorID, getShapeUnitDistance(type)}); 

    PrimitiveTransform transform = {
        .position = glm::vec4(0.0, 0.0, 0.0, 0.0),
        .rotation = glm::vec4(0.0, 0.0, 0.0, 0.0),
        .scale = glm::vec4(1.0, 1.0, 1.0, 0.0),
        .data = glm::vec4(0.0, 0.0, 0.0, 0.0),
    };
    primitiveTransforms.emplace_back(transform);
    state.needUpdate.primitiveSize = true;
}

void Primitives::bind(){

    int totalWords = (primitiveInfo.size() + 31) / 32;

    glCreateBuffers(1, &possibleHitTempleteBuffer);
    glNamedBufferStorage(possibleHitTempleteBuffer, sizeof(uint) * totalWords, (const void *)possibleHitTemplete.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, possibleHitTempleteBuffer);

    totalWords *= (state.height * state.width);

    glCreateBuffers(1, &possibleHitListBuffer);
    glNamedBufferStorage(possibleHitListBuffer, sizeof(uint) * totalWords, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, possibleHitListBuffer);

    glCreateBuffers(1, &infoBuffer);
    glNamedBufferStorage(infoBuffer, sizeof(PrimitiveInfo) * primitiveInfo.size(), (const void *)primitiveInfo.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, infoBuffer);

    glCreateBuffers(1, &transformBuffer);
    glNamedBufferStorage(transformBuffer, sizeof(PrimitiveTransform) * primitiveTransforms.size(), (const void *)primitiveTransforms.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, transformBuffer);
}

void Primitives::updatePrimitiveSize(){
    bind();
    int size = getSize();
    glNamedBufferSubData(primitiveCountUBO, 0, sizeof(int), &size);
    state.needUpdate.primitiveSize = false;
}

void Primitives::updateTransform(int index){

    GLintptr offset = index * sizeof(PrimitiveTransform);
    GLsizeiptr size = sizeof(PrimitiveTransform);

    glNamedBufferSubData(transformBuffer, offset, size, (const void*)&primitiveTransforms.at(index));
}

void Primitives::updateInfo(int index){
    GLintptr offset = index * sizeof(PrimitiveInfo);
    GLsizeiptr size = sizeof(PrimitiveInfo);

    glNamedBufferSubData(infoBuffer, offset, size, (const void*)&primitiveInfo.at(index));
}

void Primitives::updateAllInfo(){
    
    glNamedBufferSubData(infoBuffer, 0, sizeof(PrimitiveInfo)*getSize(), (const void*)primitiveInfo.data());
}

void Primitives::updateAllTransform(){

    glNamedBufferSubData(transformBuffer, 0, sizeof(PrimitiveTransform)*getSize(), (const void*)primitiveTransforms.data());
}

void Primitives::updateSpecified(){
    int index;
    while(PRIMITIVES_LEFT_UNUPDATED()){
        index = state.needUpdate.primitiveIndexs.front();
        updateTransform(index);
        updateInfo(index);
        state.needUpdate.primitiveIndexs.pop();
    }
}

void Primitives::move(int index, float xTranslation, float yTranslation, float zTranslation){

    state.needUpdate.primitiveIndexs.push(index);
    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.position[0] = xTranslation;
    shape.position[1] = yTranslation;
    shape.position[2] = zTranslation;
}

void Primitives::rotate(int index, float xRotation, float yRotation, float zRotation){

    state.needUpdate.primitiveIndexs.push(index);
    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.rotation[0] = xRotation;
    shape.rotation[1] = yRotation;
    shape.rotation[2] = zRotation;
}

void Primitives::scale(int index, float xScale, float yScale, float zScale){

    int32_t type = primitiveInfo[index].type;
    primitiveInfo[index].boundingDistance = getShapeUnitDistance((PrimitiveType)type) * std::max({xScale, yScale, zScale});

    state.needUpdate.primitiveIndexs.push(index);
    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.scale[0] = xScale;
    shape.scale[1] = yScale;
    shape.scale[2] = zScale;
}

/*
 * The bonus____Data is using leftover data in the PrimitiveTransform struct that is 
 * there for padding reasons.
 */
void Primitives::data(int index, glm::vec4 data, float bonusRotateData, float bonusScaleData, float bonusPositionData){

    state.needUpdate.primitiveIndexs.push(index);
    PrimitiveTransform& shape = primitiveTransforms.at(index);
    shape.data = data;
    shape.position[3] = bonusPositionData;
    shape.scale[3] = bonusScaleData;
    shape.rotation[3] = bonusRotateData;
}

int Primitives::getSize(){
    return primitiveInfo.size();
}
