#pragma once

#include "nn.hpp"
#include "utils.hpp"

#include <lyr/aglLayer.h>
#include <lyr/aglRenderInfo.h>

#include <cstring>

namespace sead {
class Heap;
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
        if (*mDefaultFont) return true;
        nn::os::LockMutex(&mInitMutex);
        if (*mDefaultFont == nullptr) {
            char buf[0x40];
            PRINT("Default font is null, attempting to initialize")
            mCreateDebugRenderer(mHeap, mCreateArg);
        }
        nn::os::UnlockMutex(&mInitMutex);
        return *mDefaultFont != nullptr;
    }

    const inline int version() const { return mGameVersion; }
    const inline bool isInitFont() const { return *mDefaultFont != nullptr; }

    inline void setHeap(sead::Heap* heap) { mHeap = heap; }
    inline void setFont(void** font) { mDefaultFont = font; }
    inline void setCreateCallback(void (*callback)(sead::Heap*, CreateArg&)) { mCreateDebugRenderer = callback; }
    inline void initCreateArg(int a, int b) { mCreateArg.value0 = a; mCreateArg.value1 = b; }

    inline void setDrawDebug(bool b) { mDrawDebug = b; }
    inline const bool isDrawDebug() const { return mDrawDebug; }

private:
    sead::Heap* mHeap = nullptr;
    CreateArg mCreateArg{};
    nn::os::MutexType mInitMutex;
    void** mDefaultFont = nullptr;
    void (*mCreateDebugRenderer)(sead::Heap*, CreateArg&);
    int mGameVersion;
    bool mDrawDebug = true;
};