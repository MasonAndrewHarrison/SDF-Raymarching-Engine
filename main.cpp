#define STATE_IMPLEMENTATION
#include "state.h"
#include "program.h"

int main(int argc, char* argv[]) {

    stateInit();
    Program raycaster = Program(1200, 800);
    raycaster.Running();
    raycaster.Close();

    return 0;
}