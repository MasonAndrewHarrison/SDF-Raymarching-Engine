#include "program.h"
#include "state.h"


int main(int argc, char* argv[]) {

    state = stateInit();
    Program raycaster = Program(800, 600, "Raycaster");
    raycaster.Running();
    raycaster.Close();

    return 0;
}