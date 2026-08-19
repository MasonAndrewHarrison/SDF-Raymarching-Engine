#include "program.h"


Program::Program(int width, int height){

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
    Uint64 lastTime = SDL_GetTicksNS();
    Uint64 currentTime;

    while (state.running) {

        currentTime = SDL_GetTicksNS();
        deltaTime = (double)(currentTime - lastTime)/100000000.0;
        lastTime = currentTime;

        eventHandler(&event, window, deltaTime);

        mainShader->setFloat("uCameraPhi", state.cameraPhi);
        mainShader->setFloat("uCameraDistance", state.cameraDistance);
        mainShader->setVec2("uResolution", state.width, state.height);
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