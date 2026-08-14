#ifndef STATE_H
#define STATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct State {
    bool running;
} State;

State stateInit();

extern State state;

#ifdef __cplusplus
}
#endif

#endif