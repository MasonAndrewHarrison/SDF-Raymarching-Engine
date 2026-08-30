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

void Program::running(){

    double deltaTime;
    Uint64 lastTime = SDL_GetTicks();
    Uint64 lastTimeFPS = SDL_GetTicks();
    Uint64 currentTime;
    Uint64 elapsedTimeFPS;
    int frameCount = 0;

    VoxelGrid voxelGrid = VoxelGrid({100, 100, 100});
    voxelGrid.set(0, 0, 0, 2);
    voxelGrid.bind();

    Primitives primitives = Primitives();
    primitives.append(SPHERE, 0);
    primitives.append(RECTANGLE, 1);
    primitives.append(GROUND, 2);
    primitives.append(CONE, 3);

    primitives.move(0, -1.0, 0.0, -2.0);
    primitives.move(1, 3.0, 0.0, -2.0);
    primitives.move(2, 0.0, -2.0, 0.0);
    primitives.move(3, 0, 2, 0);


    primitives.bind();

    mainShader->createCameraUBO();

    primitives.append(CONE, 1);
    primitives.move(1, 3.0, .5, 2.0);

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
        primitives.updateSpecified();
        mainShader->updateCameraUBO();

        if (state.needUpdate.primitiveSize){
            primitives.updatePrimitiveSize();
        }
       
        mainShader->use();
        screen->draw();
        SDL_GL_SwapWindow(window);
    }
}


void Program::close(){
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}