#include "debugdraw.h"
#include "callbacks.h"
#include "utils.hpp"

extern "C" void exl_main(void* x0, void* x1) {
    initDebugDrawer();
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}