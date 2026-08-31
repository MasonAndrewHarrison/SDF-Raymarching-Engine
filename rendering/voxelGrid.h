#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <stdio.h> 
#include <algorithm>

#define  VOXEL_PADDING 1

typedef struct{
    glm::ivec4 resolution;
    glm::vec4 position;
    glm::vec4 scale;
} GridMetaData;

class VoxelGrid {

    public:
        VoxelGrid(glm::ivec3 resolution);
        void set(int x, int y, int z, uint32_t colorID);
        int getIndex(int x, int y, int z);
        uint32_t get(int x, int y, int z);
        void bind();
        void sphere();
    private:
        GLuint gridBuffer;
        GLuint gridMetaDataBuffer;
        glm::ivec3 resolution;
        glm::vec3 scale;
        glm::vec3 position;
        std::vector<uint64_t> grid;
};

