#ifndef STATE_H
#define STATE_H



#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct State {
    bool running;
    int width;
    int height;
    float focusX, focusY, focusZ;
    float rayOriginX, rayOriginY, rayOriginZ;
    float cameraPhi, cameraTheda;
    float cameraDistance;
} State;

State stateInit();

extern State state;

#ifdef __cplusplus
}
#endif

#endif