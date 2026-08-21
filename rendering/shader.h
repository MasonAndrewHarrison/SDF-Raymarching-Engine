// shader.h
#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../state.h"
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();
    void use();
    unsigned int getID() const { return program; }
    void setUniforms();

private:
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, float x, float y, float z);
    void setMat4(const std::string& name, const float* matrix);
    unsigned int program;
    std::string readFile(const std::string& path);
    unsigned int compileShader(unsigned int type, const std::string& source);
};