#pragma once
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include "rendering/shader.h"
#include "rendering/screen.h"
#include "state.h"
#include <stdio.h>
#include "input/event.h"
#include "input/camera.h"

class Program{
    public:
        Program(int width, int height);
        void Running();
        void Close();
    private:
        Shader* mainShader;
        Screen* screen;
        SDL_Window* window;
        SDL_GLContext context;
        SDL_Event event;
};