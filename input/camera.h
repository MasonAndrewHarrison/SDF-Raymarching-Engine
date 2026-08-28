#ifndef CAMERA_H
#define CAMERA_H


#include "../state.h"
#include <math.h>
#include "glm/glm.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void updateRayOrigin();

struct CameraUBO {
    glm::vec4 rayOrigin;
    glm::vec2 cameraAngle;
    glm::ivec2 resolution;
};

inline void updateRayOrigin(){
    state.rayOriginZ = state.cameraDistance * sin(state.cameraTheda + M_PI/2) * cos(state.cameraPhi) + state.focusZ;
    state.rayOriginY = state.cameraDistance * cos(state.cameraTheda + M_PI/2) + state.focusY;
    state.rayOriginX = state.cameraDistance * sin(state.cameraTheda + M_PI/2) * sin(state.cameraPhi) + state.focusX;
}


#endif