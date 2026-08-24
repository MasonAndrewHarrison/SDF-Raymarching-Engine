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
    const bool* keyboardState;
} State;

void stateInit(void);

extern State state;

#ifdef __cplusplus
}
#endif

#endif // STATE_H


#ifdef STATE_IMPLEMENTATION
#ifndef STATE_IMPLEMENTATION_DONE
#define STATE_IMPLEMENTATION_DONE

State state;

void stateInit(void) {
    State s = {0};
    s.running   = true;
    s.width     = -1;
    s.height    = -1;
    s.rayOriginZ = -3.0f;
    s.cameraDistance = -3.0f;
    state = s;
}

#endif // STATE_INPLEMENTATION_DONE
#endif // STATE_IMPLEMENTATION