#pragma once

#include "types.h"

// general

static constexpr u64 sGetVirtualAddressHeapOffsets[6] = {
    // 1.1.2 has a very nice looking address
    0x010e7fe0, 0x01119a80, 0x0111ba30, 0x01111100, 0x01109730, 0x01105300
};

static constexpr u64 sSetDebugHeapOffsets[6] = {
    0x00ea5118, 0x00adb7cc, 0x00b03db8, 0x00a9c6b4, 0x00ab0150, 0x00a9123c
};

static constexpr u64 sEnableDebugDrawOffsets[6] = {
    0x00857f28, 0x0083f5b8, 0x00784e90, 0x00808478, 0x007d1fb0, 0x00818340
};

static constexpr u64 sDebugDrawOffsets[6] = {
    0x00858998, 0x0084002c, 0x007859e0, 0x00808efc, 0x007d2ae4, 0x0081911c
};

// sead::TextWriter + font stuff

static constexpr u64 sTextWriterPrintfOffsets[6] = {
    0x0184c9e0, 0x018993e4, 0x01899004, 0x0188e2c8, 0x0187ab3c, 0x018890d4
};

static constexpr u64 sTextWriterCtorOffsets[6] = {
    0x0108ed1c, 0x010bfdf8, 0x010c85b8, 0x010b4648, 0x010ab4d8, 0x010ad510
};

static constexpr u64 sTextWriterSetupGraphicsOffsets[6] = {
    0x0184c778, 0x0189917c, 0x01898d9c, 0x0188e060, 0x0187a8d4, 0x01888e6c
};

static constexpr u64 sDefaultFontOffsets[6] = {
    0x0463c0f8, 0x04719618, 0x04721548, 0x04713a28, 0x04707e80, 0x04716af8
};

// sead::PrimitiveRenderer + related

static constexpr u64 sPrimitiveRendererOffsets[6] = {
    0, 0, 0, 0, 0, 0x04718c80
};

static constexpr u64 sBeginOffsets[6] = {
    0, 0, 0, 0, 0, 0x022707c8
};

static constexpr u64 sEndOffsets[6] = {
    0, 0, 0, 0, 0, 0x022707f0
};

// sead::PrimitiveDrawer::drawQuad(const sead::PrimitiveDrawer::QuadArg&)
static constexpr u64 sDrawQuadOffsets[6] = {
    0, 0, 0, 0, 0, 0x021c3654
};

// sead::PrimitiveDrawer::drawQuad(const sead::Color4f&, const sead::Color4f&)
static constexpr u64 sDrawQuad2Offsets[6] = {
    0, 0, 0, 0, 0, 0x022708d8
};

static constexpr u64 sDrawBoxOffsets[6] = {
    0, 0, 0, 0, 0, 0x02270904
};

static constexpr u64 sDrawWireCubeOffsets[6] = {
    0, 0, 0, 0, 0, 0x021c372c
};

static constexpr u64 sDrawLineOffsets[6] = {
    0, 0, 0, 0, 0, 0x021c37bc
};

static constexpr u64 sDrawSphere4x8Offsets[6] = {
    0, 0, 0, 0, 0, 0x021c393c
};

static constexpr u64 sDrawSphere8x16Offsets[6] = {
    0, 0, 0, 0, 0, 0x021c39d8
};

static constexpr u64 sDrawDisk32Offsets[6] = {
    0, 0, 0, 0, 0, 0x021c3a7c
};

static constexpr u64 sDrawCircle32Offsets[6] = {
    0, 0, 0, 0, 0, 0x021c3b20
};

static constexpr u64 sDrawCylinder16Offsets[6] = {
    0, 0, 0, 0, 0, 0x021c3bac
};

static constexpr u64 sDrawCylinder32Offsets[6] = {
    0, 0, 0, 0, 0, 0x021c3c48
};

static constexpr u64 sSetModelMtxOffsets[6] = {
    0, 0, 0, 0, 0, 0x02270118
};

static constexpr u64 sSetProjectionOffsets[6] = {
    0, 0, 0, 0, 0, 0x021c362c
};

static constexpr u64 sSetCameraOffsets[6] = {
    0, 0, 0, 0, 0, 0x0227010c
};

static constexpr u64 sSetDrawCtxOffsets[6] = {
    0, 0, 0, 0, 0, 0x022707c0
};

static constexpr u64 sGetResidentActorMgrOffsets[6] = {
    0, 0, 0, 0, 0, 0x00cdd728
};

static constexpr u64 sFormalSceneOffsets[6] = {
    0, 0, 0, 0, 0, 0x010d5024
};