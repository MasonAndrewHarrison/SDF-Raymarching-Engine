#define STATE_IMPLEMENTATION
#include "state.h"
#include "program.h"

int main(int argc, char* argv[]) {

    stateInit();
    Program raycaster = Program(1200, 800);
    raycaster.running();
    raycaster.close();

    return 0;
}