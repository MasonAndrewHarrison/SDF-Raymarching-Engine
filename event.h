#ifndef EVENT_H
#define EVENT_H
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include "state.h"
#include "stdio.h"


#ifdef __cplusplus
extern "C" {
#endif

void eventHandler(SDL_Event* event, SDL_Window* window, double deltaTime);

#ifdef __cplusplus
}
#endif

#endif 