#include "event.h"

static void eventButtonPress(SDL_Event* event);
static void eventButtonHold(double deltaTime);
static void eventMouseMotion(SDL_Event* event);
static void eventMouseWheel(SDL_Event* event);

void eventHandler(SDL_Event* event, SDL_Window* window, double deltaTime){

    state.keyboardState = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_EVENT_QUIT) {
            state.running = false;
        }

        if (event->type == SDL_EVENT_KEY_DOWN) {
            eventButtonPress(event);
        }

        if (event->type == SDL_EVENT_MOUSE_MOTION){
            eventMouseMotion(event);
        }

        if (event->type == SDL_EVENT_MOUSE_WHEEL){
            eventMouseWheel(event);
        }

        if (event->type == SDL_EVENT_WINDOW_RESIZED) {
            SDL_GetWindowSizeInPixels(window, &state.width, &state.height);
            glViewport(0, 0, state.width, state.height);
        }
    }
    eventButtonHold(deltaTime);
}

static void eventButtonPress(SDL_Event* event){

    switch (event->key.key){

        case SDLK_ESCAPE:
            state.running = false;
            break;
    }   
}

static void eventButtonHold(double deltaTime){

    float speed = 10;

    if (state.keyboardState[SDL_SCANCODE_W]){
        state.focusX += speed * deltaTime;
    }
    if (state.keyboardState[SDL_SCANCODE_S]){
        state.focusX -= speed * deltaTime;
    }
    if (state.keyboardState[SDL_SCANCODE_Q]){
        state.focusY -= speed * deltaTime;
    }
    if (state.keyboardState[SDL_SCANCODE_E]){
        state.focusY += speed * deltaTime;
    }
    if (state.keyboardState[SDL_SCANCODE_A]){
        state.focusZ -= speed * deltaTime;
    }
    if (state.keyboardState[SDL_SCANCODE_D]){
        state.focusZ += speed * deltaTime;
    }

}

static void eventMouseMotion(SDL_Event* event){

    float sensitivity = 0.005f;  

    if (event->button.button == SDL_BUTTON_MIDDLE) {
        state.cameraTheda += event->motion.yrel * sensitivity;
        state.cameraPhi   += event->motion.xrel * sensitivity;
    }

    if (state.cameraPhi > 89.0f)  state.cameraPhi = 89.0f;
    if (state.cameraPhi < -89.0f) state.cameraPhi = -89.0f;
}

static void eventMouseWheel(SDL_Event* event){

    float zoomSpeed = 0.5f;

    if (state.keyboardState[SDL_SCANCODE_X]){
        state.focusX += event->wheel.y * zoomSpeed;
    }
    else if (state.keyboardState[SDL_SCANCODE_Z]){
        state.focusY += event->wheel.y * zoomSpeed;
    }
    else if (state.keyboardState[SDL_SCANCODE_Y]){
        state.focusZ += event->wheel.y * zoomSpeed;
    }
    else {
        state.cameraDistance += event->wheel.y * zoomSpeed;
    }
}