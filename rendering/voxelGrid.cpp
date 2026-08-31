#include "voxelGrid.h"


VoxelGrid::VoxelGrid(glm::ivec3 resolution){

    this->resolution = resolution;
    this->position = glm::vec3(0.0, 0.0, 0.0);
    this->scale = glm::vec3(1.0, 1.0, 1.0);

    resolution += (VOXEL_PADDING*2);
    grid.resize(resolution.x * resolution.y * resolution.z);
}

int VoxelGrid::getIndex(int x, int y, int z){
    x += resolution.x/2;
    y += resolution.y/2;
    z += resolution.z/2;
    return x + y * resolution.x + z * resolution.x * resolution.y;
}

void VoxelGrid::set(int x, int y, int z, uint32_t colorID){
    grid[getIndex(x+VOXEL_PADDING, y+VOXEL_PADDING, z+VOXEL_PADDING)] = colorID;
}

void VoxelGrid::bind(){

    GridMetaData gridMetaData = {
        .resolution = glm::ivec4(resolution, 0),
        .position = glm::vec4(position, 0.0),
        .scale = glm::vec4(scale, 0.0),
    };

    glCreateBuffers(1, &gridBuffer);
    glCreateBuffers(1, &gridMetaDataBuffer);

    glNamedBufferStorage(gridBuffer, sizeof(uint64_t)*grid.size(), (const void *)grid.data(), GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(gridMetaDataBuffer, sizeof(GridMetaData), &gridMetaData, GL_DYNAMIC_STORAGE_BIT);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, gridBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, gridMetaDataBuffer);

}