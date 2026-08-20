#include "state.h"

State state;


State stateInit() {
    return (State){
        .running        = true,
        .height         = -1,
        .width          = -1,
        .cameraDistance = -3,
        .rayOriginZ     = -3,
    };
}