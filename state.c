#include "state.h"

State state;

State stateInit(){

    State state;
    state.running = true;
    state.height = -1;
    state.width = -1;
    state.cameraDistance = -3;
    state.focusX = 0;
    state.focusY = 0;
    state.focusZ = 0;
    state.cameraPhi = 0;
    state.cameraTheda = 0;

    return state;
}