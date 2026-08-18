#include "program.h"
#include "state.h"


int main(int argc, char* argv[]) {

    state = stateInit();
    Program raycaster = Program(1200, 800);
    raycaster.Running();
    raycaster.Close();

    return 0;
}