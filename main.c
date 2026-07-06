#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH               800
#define HEIGHT              600
#define MAX_SHADER_ERROR    1024

static void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void glCheckError(const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        switch(error) {
            case GL_INVALID_ENUM:
                printf("[%s:%d] GL_INVALID_ENUM\n", file, line);
                break;
            case GL_INVALID_VALUE:
                printf("[%s:%d] GL_INVALID_VALUE\n", file, line);
                break;
            case GL_INVALID_OPERATION:
                printf("[%s:%d] GL_INVALID_OPERATION\n", file, line);
                break;
            case GL_OUT_OF_MEMORY:
                printf("[%s:%d] GL_OUT_OF_MEMORY\n", file, line);
                break;
            default:
                printf("[%s:%d] Error: %u\n", file, line, error);
                break;
        }
    }
}

#define GL_CALL(x) glClearError(); x; glCheckError(__FILE__, __LINE__);

char* readFile(const char* path) {

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        printf("error: could not open file %s\n", path);
        return NULL;
    }


    fseek(file, 0, SEEK_END);   
    long length = ftell(file);  
    rewind(file);          

    char* buffer = malloc(length + 1);
    if (buffer == NULL) {
        printf("error: malloc failed for file %s\n", path);
        fclose(file);
        return NULL;
    }


    fread(buffer, 1, length, file);
    buffer[length] = '\0';  

    fclose(file);
    return buffer;
}

static unsigned int compileShader(unsigned int type, const char* source){
    
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        char message[MAX_SHADER_ERROR];
        glGetShaderInfoLog(id, MAX_SHADER_ERROR, NULL, message);
        printf("%s", message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const char* vertexShader, const char* fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void) {

    if (!glfwInit()) return -1;

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Cuastic Gaussian Demo", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window); 
    glewExperimental = GL_TRUE;
    glewInit();

    printf("%s\n", glGetString(GL_VERSION));

    float position[] = {
        -0.8f, -0.8f,
        0.8f, -0.8f,
        0.8f, 0.8f,
        -0.8f, 0.8f,

    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,

    };

    unsigned int buffer;  
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), position, GL_STATIC_DRAW); 

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);

    unsigned int ibo;
    glGenBuffers(1, &ibo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); 


    char* vertexSource = readFile("shaders/vertex.glsl");
    char* fragmentSource = readFile("shaders/fragment.glsl");
    unsigned int shader = createShader(vertexSource, fragmentSource);
    free(vertexSource);
    free(fragmentSource);
    glUseProgram(shader);

    int timeLoc = glGetUniformLocation(shader, "uTime");
    int resolutionLoc = glGetUniformLocation(shader, "uResolution");

    float startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime() - startTime;
        glUniform1f(timeLoc, currentTime);
        glUniform2f(resolutionLoc, WIDTH, HEIGHT);

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
} 