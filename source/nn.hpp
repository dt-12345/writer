#pragma once

// Force def as module
#define EXL_AS_MODULE

/* nnSdk is only available when loaded as a module. */
#ifndef EXL_AS_MODULE
#error "Cannot use nnSdk when not as a module!"
#endif

#include "nn/crypto.hpp"
#include "nn/fs.hpp"
#include "nn/oe.hpp"
#include "nn/os.hpp"
#include "nn/time.hpp"