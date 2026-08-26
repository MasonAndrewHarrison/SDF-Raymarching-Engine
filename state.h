#ifndef STATE_H
#define STATE_H



#include <stdbool.h>
#include <queue>

typedef struct {
    bool primitiveCount;
    std::queue<int> primitiveIndexs;
} NeedUpdate;

typedef struct State {
    bool running;
    int width;
    int height;
    float focusX, focusY, focusZ;
    float rayOriginX, rayOriginY, rayOriginZ;
    float cameraPhi, cameraTheda;
    float cameraDistance;
    const bool* keyboardState;
    NeedUpdate needUpdate;
} State;

void stateInit(void);

extern State state;

#define PRIMITIVES_LEFT_UNUPDATED() \
    (!(state).needUpdate.primitiveIndexs.empty())

#endif // STATE_H


#ifdef STATE_IMPLEMENTATION
#ifndef STATE_IMPLEMENTATION_DONE
#define STATE_IMPLEMENTATION_DONE

State state;

void stateInit(void) {
    NeedUpdate needUpdate = {0};
    needUpdate.primitiveCount = true;


    State s = {0};
    s.running   = true;
    s.width     = -1;
    s.height    = -1;
    s.rayOriginZ = -3.0f;
    s.cameraDistance = -3.0f;
    s.needUpdate = needUpdate;
    state = s;
}



#endif // STATE_INPLEMENTATION_DONE
#endif // STATE_IMPLEMENTATION

