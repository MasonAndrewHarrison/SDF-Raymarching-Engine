#include "program.h"
#include "state.h"


int main(int argc, char* argv[]) {

    state = stateInit();
    Program raycaster = Program(600, 400);
    raycaster.Running();
    raycaster.Close();

    return 0;
}