#include "program.h"


Program::Program(int width, int height){

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    state.width = width;
    state.height = height;

    window = SDL_CreateWindow(
        "Raycaster -dev",
        width, height,
        SDL_WINDOW_OPENGL
    );
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    glewExperimental = GL_TRUE;
    glewInit();
    SDL_GL_SetSwapInterval(0);

    screen = new Screen();
    mainShader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

}

void Program::Running(){

    double deltaTime;
    Uint64 lastTime = SDL_GetTicks();
    Uint64 lastTimeFPS = SDL_GetTicks();
    Uint64 currentTime;
    Uint64 elapsedTimeFPS;
    int frameCount;


    Primitives primitives = Primitives();
    primitives.Append(SPHERE, 0, glm::vec3{-1.0, 1.0, 1.0}, glm::vec3{1.0, 1.0, 1.0});
    primitives.Append(RECTANGLE, 1, glm::vec3{1.3, -1.1, 0.0}, glm::vec3{2.0, 2.0, 1.0});
    primitives.Append(GROUND, 2, glm::vec3{1.0, -1.0, 0.0}, glm::vec3{2.0, 3.0, 1.0});
    primitives.Bind();

    mainShader->createCameraUBO();

    while (state.running) {

        currentTime = SDL_GetTicks();
        deltaTime = (double)(currentTime - lastTime)/1000.0f;
        lastTime = currentTime;

        frameCount++;
        elapsedTimeFPS = currentTime - lastTimeFPS;
        if (elapsedTimeFPS >= 1000){
            std::cout << "FPS: " << (frameCount * 1000.0f) / elapsedTimeFPS << std::endl;
            frameCount = 0;
            lastTimeFPS = currentTime;
        }

        eventHandler(&event, window, deltaTime);
        updateRayOrigin();

        
        mainShader->updateCameraUBO();
        mainShader->use();
        screen->draw();
        SDL_GL_SwapWindow(window);
    }
}


void Program::Close(){
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}