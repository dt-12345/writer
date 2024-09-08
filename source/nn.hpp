#pragma once

// Force def as module
#define EXL_AS_MODULE

/* nnSdk is only available when loaded as a module. */
#ifndef EXL_AS_MODULE
#error "Cannot use nnSdk when not as a module!"
#endif

#include "nn/fs.h"
#include "nn/oe.h"
#include "nn/os.h"
#include "nn/time.hpp"