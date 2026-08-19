#include "event.h"

static void eventButtonPress(SDL_Event* event, double deltaTime);
static void eventButtonHold(const bool* stateArray, double deltaTime);

void eventHandler(SDL_Event* event, SDL_Window* window, double deltaTime){

    while (SDL_PollEvent(event)) {
        if (event->type == SDL_EVENT_QUIT) {
            state.running = false;
        }

        if (event->type == SDL_EVENT_KEY_DOWN) {
            eventButtonPress(event, deltaTime);
        }

        if (event->type == SDL_EVENT_WINDOW_RESIZED) {
            SDL_GetWindowSizeInPixels(window, &state.width, &state.height);
            glViewport(0, 0, state.width, state.height);

        }
    }
    const bool* stateArray = SDL_GetKeyboardState(NULL);
    eventButtonHold(stateArray, deltaTime);
}

static void eventButtonPress(SDL_Event* event, double deltaTime){

    switch (event->key.key){

        case SDLK_ESCAPE:
            state.running = false;
            break;
    }   
}

static void eventButtonHold(const bool* stateArray, double deltaTime){

    if (stateArray[SDL_SCANCODE_W]){
        state.cameraDistance += 1 * deltaTime;
    }
    if (stateArray[SDL_SCANCODE_S]){
        state.cameraDistance -= 1 * deltaTime;
    }
    if (stateArray[SDL_SCANCODE_A]){
        state.cameraPhi -= 1 * deltaTime;
    }
    if (stateArray[SDL_SCANCODE_D]){
        state.cameraPhi += 1 * deltaTime;
    }
}