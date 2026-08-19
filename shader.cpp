// shader.cpp
#include "shader.h"


std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "error: could not open file " << path << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char message[1024];
        glGetShaderInfoLog(id, 1024, nullptr, message);
        std::cerr << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader error: " << message << "\n";
        glDeleteShader(id);
        return 0;
    }
    return id;
}

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    std::string vertSrc = readFile(vertPath);
    std::string fragSrc = readFile(fragPath);

    program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertSrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader(){
    glDeleteProgram(program);
}

void Shader::setUniforms(){

    glm::vec3 rayOrigin;

    this->setFloat("uCameraPhi", state.cameraPhi);
    this->setFloat("uCameraTheda", state.cameraTheda);
    this->setFloat("uCameraDistance", state.cameraDistance);
    this->setVec2("uResolution", state.width, state.height);
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setMat4(const std::string& name, const float* matrix) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix);
}
