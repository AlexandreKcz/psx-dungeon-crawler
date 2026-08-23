#include "src/engine/engine.h"

/// main framework loop
int main(void) {
    _engine_initialize();
    _engine_start();

    while(1){
        _engine_update();
    }
}