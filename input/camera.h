#ifndef CAMERA_H
#define CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../state.h"
#include "math.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
void updateRayOrigin();

inline void updateRayOrigin(){
    state.rayOriginZ = state.cameraDistance * sin(state.cameraTheda + M_PI/2) * cos(state.cameraPhi) + state.focusZ;
    state.rayOriginY = state.cameraDistance * cos(state.cameraTheda + M_PI/2) + state.focusY;
    state.rayOriginX = state.cameraDistance * sin(state.cameraTheda + M_PI/2) * sin(state.cameraPhi) + state.focusX;
}

#ifdef __cplusplus
}
#endif

#endif