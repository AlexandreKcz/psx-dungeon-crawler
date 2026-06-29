#include "src/engine/engine.h"

int main(void) {
    initialize();
    start();

    while(1){
        update();
    }
}