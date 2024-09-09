#pragma once

#include "nn.hpp"
#include "utils.hpp"

#include <lyr/aglLayer.h>
#include <lyr/aglRenderInfo.h>

#include <cstring>

namespace sead {
class Heap;
class FontBase;
} // namespace sead

// GraphicsModule CreateArg
struct CreateArg {
    char _whatever[0xb4c];
    int value0;
    char _whatever2[0x10];
    int value1;
};

class DebugDrawMgr {
public:
    void initialize(int version) {
        mGameVersion = version;
        nn::os::InitializeMutex(&mInitMutex, true, 0);
    }

    inline bool createDebugRenderer() {
        if (isInitFont()) return true;
        nn::os::LockMutex(&mInitMutex);
        if (!isInitFont()) {
            char buf[0x40];
            PRINT("Default font is null, attempting to initialize")
            mCreateDebugRenderer(mHeap, mCreateArg);
        }
        nn::os::UnlockMutex(&mInitMutex);
        return isInitFont();
    }

    const inline int version() const { return mGameVersion; }
    const inline bool isInitFont() const { return *mDefaultFont != nullptr; }

    inline void setHeap(sead::Heap* heap) { mHeap = heap; }
    inline void setFont(sead::FontBase** font) { mDefaultFont = font; }
    inline void setCreateCallback(void (*callback)(sead::Heap*, CreateArg&)) { mCreateDebugRenderer = callback; }
    inline void initCreateArg(int a, int b) { mCreateArg.value0 = a; mCreateArg.value1 = b; }

    inline void setDrawDebug(bool b) { mDrawDebug = b; }
    inline void setDrawUI2D(bool b) { mDrawUI2D = b; }
    inline void setDrawUI2DOverlay(bool b) { mDrawUI2DOverlay = b; }
    inline void setDrawTool2D(bool b) { mDrawTool2D = b; }
    inline void setDrawTool2DSuper(bool b) { mDrawTool2DSuper = b; }
    inline void setDrawMain3D0(bool b) { mDrawMain3D0 = b; }
    inline void setDrawUI3D0(bool b) { mDrawUI3D0 = b; }

    inline const bool isDrawDebug() const { return mDrawDebug; }
    inline const bool isDrawUI2D() const { return mDrawUI2D; }
    inline const bool isDrawUI2DOverlay() const { return mDrawUI2DOverlay; }
    inline const bool isDrawTool2D() const { return mDrawTool2D; }
    inline const bool isDrawTool2DSuper() const { return mDrawTool2DSuper; }
    inline const bool isDrawMain3D0() const { return mDrawMain3D0; }
    inline const bool isDrawUI3D0() const { return mDrawUI3D0; }

private:
    sead::Heap* mHeap = nullptr;
    CreateArg mCreateArg{};
    nn::os::MutexType mInitMutex;
    sead::FontBase** mDefaultFont = nullptr;
    void (*mCreateDebugRenderer)(sead::Heap*, CreateArg&);
    int mGameVersion;
    bool mDrawDebug = true;
    bool mDrawUI2D = true;
    bool mDrawUI2DOverlay = true;
    bool mDrawTool2D = true;
    bool mDrawTool2DSuper = true;
    bool mDrawMain3D0 = true;
    bool mDrawUI3D0 = true;
};