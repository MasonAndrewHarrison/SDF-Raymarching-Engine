#include "camera.h"

void updateRayOrigin(){

    state.rayOriginZ = state.cameraDistance * sin(state.cameraTheda + M_PI/2) * cos(state.cameraPhi) + state.focusZ;
    state.rayOriginY = state.cameraDistance * cos(state.cameraTheda + M_PI/2) + state.focusY;
    state.rayOriginX = state.cameraDistance * sin(state.cameraTheda + M_PI/2) * sin(state.cameraPhi) + state.focusX;

}