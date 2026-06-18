#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdio.h>


static unsigned int compileShader(unsigned int type, const char* source){
    
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
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

    //Read doc on these.
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cuastic Gaussian Demo", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window); 
    glewExperimental = GL_TRUE;
    glewInit();

    printf("%s", glGetString(GL_VERSION));

    float position[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f,
    };

    unsigned int buffer;  
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW); 

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const char* vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;\n"
        "void main() {\n"
        "    gl_Position = position;\n"
        "}\n";

    const char* fragmentShader =
        "#version 330 core\n"
        "layout(location = 0) out vec4 color;\n"
        "void main() {\n"
        "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
        
    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
} 