#pragma once

#include "types.h"

static constexpr u64 sTextWriterPrintfOffsets[6] = {
    0x0184c9e0, 0x018993e4, 0x01899004, 0x0188e2c8, 0x0187ab3c, 0x018890d4
};

static constexpr u64 sTextWriterCtorOffsets[6] = {
    0x0108ed1c, 0x010bfdf8, 0x010c85b8, 0x010b4648, 0x010ab4d8, 0x010ad510
};

static constexpr u64 sTextWriterSetupGraphicsOffsets[6] = {
    0x0184c778, 0x0189917c, 0x01898d9c, 0x0188e060, 0x0187a8d4, 0x01888e6c
};

static constexpr u64 sCreateDebugRendererOffsets[6] = {
    0x00ea6834, 0x00adcef4, 0x00b054d8, 0x00a9dddc, 0x00ab1870, 0x00a92964
};

static constexpr u64 sDefaultFontOffsets[6] = {
    0x0463c0f8, 0x04719618, 0x04721548, 0x04713a28, 0x04707e80, 0x04716af8
};

static constexpr u64 sStealHeapOffsets[6] = {
    0x008111a8, 0x0083c2fc, 0x00713b74, 0x00802bfc, 0x007a3428, 0x007f61d0
};

static constexpr u64 sGetCreateArgOffsets[6] = {
    0x00ea5118, 0x00adb7cc, 0x00b03db8, 0x00a9c6b4, 0x00ab0150, 0x00a9123c
};

static constexpr u64 sEnableDebugDrawOffsets[6] = {
    0x00857f28, 0x0083f5b8, 0x00784e90, 0x00808478, 0x007d1fb0, 0x00818340
};

static constexpr u64 sDebugDrawOffsets[6] = {
    0x00858998, 0x0084002c, 0x007859e0, 0x00808efc, 0x007d2ae4, 0x0081911c
};