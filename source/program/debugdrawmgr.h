#pragma once

#include "nn.hpp"
#include "utils.hpp"

#include "offsets.h"

#include <common/aglDrawContext.h>
#include <gfx/seadColor.h>
#include <gfx/seadPrimitiveRenderer.h>
#include <lyr/aglLayer.h>
#include <lyr/aglRenderInfo.h>
#include <math/seadMatrix.h>

#include <cstring>

namespace sead {
class Camera;
class DrawContext;
class FontBase;
class Heap;
class Projection;
} // namespace sead

// GraphicsModule CreateArg
struct CreateArg {
    char _whatever[0xb4c];
    int value0;
    char _whatever2[0x10];
    int value1;
};

// missing Disk16 and Circle16, don't seem to exist - could implement them ourselves but the others don't even work yet so whatever
using BeginFunc = void (sead::PrimitiveDrawer*);
using EndFunc = void (sead::PrimitiveDrawer*);
using DrawQuadFunc = void (sead::PrimitiveDrawer*, const sead::PrimitiveDrawer::QuadArg&);
using DrawQuad2Func = void (sead::PrimitiveDrawer*, const sead::Color4f&, const sead::Color4f&);
using DrawBoxFunc = void (sead::PrimitiveDrawer*, const sead::Color4f&, const sead::Color4f&);
using DrawWireCubeFunc = void (sead::PrimitiveDrawer*, const sead::PrimitiveDrawer::CubeArg&);
using DrawLineFunc = void (sead::PrimitiveDrawer*, const sead::Vector3f&, const sead::Vector3f&, const sead::Color4f&, const sead::Color4f&);
using DrawSphere4x8Func = void (sead::PrimitiveDrawer*, const sead::Vector3f&, float, const sead::Color4f&, const sead::Color4f&);
using DrawSphere8x16Func = void (sead::PrimitiveDrawer*, const sead::Vector3f&, float, const sead::Color4f&, const sead::Color4f&);
using DrawDisk32Func = void (sead::PrimitiveDrawer*, const sead::Vector3f&, float, const sead::Color4f&, const sead::Color4f&);
using DrawCircle32Func = void (sead::PrimitiveDrawer*, const sead::Vector3f&, float, const sead::Color4f&);
using DrawCylinder16Func = void (sead::PrimitiveDrawer*, const sead::Vector3f&, float, float, const sead::Color4f&, const sead::Color4f&);
using DrawCylinder32Func = void (sead::PrimitiveDrawer*, const sead::Vector3f&, float, float, const sead::Color4f&, const sead::Color4f&);
using SetModelMtxFunc = void (sead::PrimitiveDrawer*, sead::Matrix34f*);
using SetProjectionFunc = void (sead::PrimitiveDrawer*, sead::Projection*);
using SetCameraFunc = void (sead::PrimitiveDrawer*, sead::Camera*);
using SetDrawCtxFunc = void (sead::PrimitiveDrawer*, sead::DrawContext*);

class DebugDrawMgr {
public:
    void initialize(int version) {
        mGameVersion = version;
        nn::os::InitializeMutex(&mInitMutex, true, 0);

        #define OFFSET(offsets) exl::util::modules::GetTargetOffset((offsets)[this->version()])

        Begin = reinterpret_cast<BeginFunc*>(OFFSET(sBeginOffsets));
        End = reinterpret_cast<EndFunc*>(OFFSET(sEndOffsets));
        DrawQuad = reinterpret_cast<DrawQuadFunc*>(OFFSET(sDrawQuadOffsets));
        DrawQuad2 = reinterpret_cast<DrawQuad2Func*>(OFFSET(sDrawQuad2Offsets));
        DrawBox = reinterpret_cast<DrawBoxFunc*>(OFFSET(sDrawBoxOffsets));
        DrawWireCube = reinterpret_cast<DrawWireCubeFunc*>(OFFSET(sDrawWireCubeOffsets));
        DrawLine = reinterpret_cast<DrawLineFunc*>(OFFSET(sDrawLineOffsets));
        DrawSphere4x8 = reinterpret_cast<DrawSphere4x8Func*>(OFFSET(sDrawSphere4x8Offsets));
        DrawSphere8x16 = reinterpret_cast<DrawSphere8x16Func*>(OFFSET(sDrawSphere8x16Offsets));
        DrawDisk32 = reinterpret_cast<DrawDisk32Func*>(OFFSET(sDrawDisk32Offsets));
        DrawCircle32 = reinterpret_cast<DrawCircle32Func*>(OFFSET(sDrawCircle32Offsets));
        DrawCylinder16 = reinterpret_cast<DrawCylinder16Func*>(OFFSET(sDrawCylinder16Offsets));
        DrawCylinder32 = reinterpret_cast<DrawCylinder32Func*>(OFFSET(sDrawCylinder32Offsets));
        SetModelMtx = reinterpret_cast<SetModelMtxFunc*>(OFFSET(sSetModelMtxOffsets));
        SetProjection = reinterpret_cast<SetProjectionFunc*>(OFFSET(sSetProjectionOffsets));
        SetCamera = reinterpret_cast<SetCameraFunc*>(OFFSET(sSetCameraOffsets));
        SetDrawCtx = reinterpret_cast<SetDrawCtxFunc*>(OFFSET(sSetDrawCtxOffsets));

        #undef OFFSET
    }

    inline bool createDebugRenderer() {
        if (isInitFont() && isInitPrimitiveRenderer()) return true;
        nn::os::LockMutex(&mInitMutex);
        if (!isInitFont() || !isInitPrimitiveRenderer()) {
            char buf[0x40];
            PRINT("Debug renderer is null, attempting to initialize")
            mCreateDebugRenderer(mHeap, mCreateArg);
        }
        nn::os::UnlockMutex(&mInitMutex);
        return isInitFont() && isInitPrimitiveRenderer();
    }

    const inline int version() const { return mGameVersion; }
    const inline bool isInitFont() const { return *mDefaultFont != nullptr; }
    const inline bool isInitPrimitiveRenderer() const { return *mPrimitiveRenderer != nullptr; }

    const inline sead::FontBase* getFont() const { return *mDefaultFont; }
    inline sead::PrimitiveRenderer* getPrimitiveRenderer() { return *mPrimitiveRenderer; }
    inline sead::PrimitiveDrawer* getPrimitiveDrawer() { return getPrimitiveRenderer()->getPrimitiveDrawer(); }

    inline void setHeap(sead::Heap* heap) { mHeap = heap; }
    inline void setFont(sead::FontBase** font) { mDefaultFont = font; }
    inline void setPrimitiveRenderer(sead::PrimitiveRenderer** renderer) { mPrimitiveRenderer = renderer; }
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

    void setupRenderer(const agl::lyr::RenderInfo& info) {
        setProjection(info.projection);
        setCamera(info.camera);
        setModelMtx(&mMatrix);
        setDrawCtx(dynamic_cast<sead::DrawContext*>(info.draw_ctx));
    }

    // PrimitiveRenderer's draw functions don't exist in totk, we only have PrimitiveDrawer bc of aal::DebugDrawer
    void begin() {
        Begin(getPrimitiveDrawer());
    }
    void end() {
        End(getPrimitiveDrawer());
    }
    void drawQuad(const sead::PrimitiveDrawer::QuadArg& arg) {
        DrawQuad(getPrimitiveDrawer(), arg);
    }
    void drawQuad(const sead::Color4f& c0, const sead::Color4f& c1) {
        DrawQuad2(getPrimitiveDrawer(), c0, c1);
    }
    void drawBox(const sead::Color4f& c0, const sead::Color4f& c1) {
        DrawBox(getPrimitiveDrawer(), c0, c1);
    }
    void drawWireCube(const sead::PrimitiveDrawer::CubeArg& arg) {
        DrawWireCube(getPrimitiveDrawer(), arg);
    }
    void drawLine(const sead::Vector3f& start, const sead::Vector3f& end, const sead::Color4f& c0, const sead::Color4f& c1) {
        DrawLine(getPrimitiveDrawer(), start, end, c0, c1);
    }
    void drawSphere4x8(const sead::Vector3f& pos, float radius, const sead::Color4f& north, const sead::Color4f& south) {
        DrawSphere4x8(getPrimitiveDrawer(), pos, radius, north, south);
    }
    void drawSphere8x16(const sead::Vector3f& pos, float radius, const sead::Color4f& north, const sead::Color4f& south) {
        DrawSphere8x16(getPrimitiveDrawer(), pos, radius, north, south);
    }
    void drawDisk32(const sead::Vector3f& pos, float radius, const sead::Color4f& center, const sead::Color4f& edge) {
        DrawDisk32(getPrimitiveDrawer(), pos, radius, center, edge);
    }
    void drawCircle32(const sead::Vector3f& pos, float radius, const sead::Color4f& color) {
        DrawCircle32(getPrimitiveDrawer(), pos, radius, color);
    }
    void drawCylinder16(const sead::Vector3f& pos, float radius, float height, const sead::Color4f& top, const sead::Color4f& btm) {
        DrawCylinder16(getPrimitiveDrawer(), pos, radius, height, top, btm);
    }
    void drawCylinder32(const sead::Vector3f& pos, float radius, float height, const sead::Color4f& top, const sead::Color4f& btm) {
        DrawCylinder32(getPrimitiveDrawer(), pos, radius, height, top, btm);
    }
    void drawAxis(const sead::Vector3f& pos, float size) {
        drawLine(pos, sead::Vector3f(pos.x + size, pos.y, pos.z), sead::Color4f(1.f, 0.f, 0.f, 1.f), sead::Color4f(1.f, 0.f, 0.f, 1.f));
        drawLine(pos, sead::Vector3f(pos.x, pos.y + size, pos.z), sead::Color4f(0.f, 1.f, 0.f, 1.f), sead::Color4f(0.f, 1.f, 0.f, 1.f));
        drawLine(pos, sead::Vector3f(pos.x, pos.y, pos.z + size), sead::Color4f(0.f, 0.f, 1.f, 1.f), sead::Color4f(0.f, 0.f, 1.f, 1.f));
    }
    void setModelMtx(sead::Matrix34f* mtx) {
        SetModelMtx(getPrimitiveDrawer(), mtx);
    }
    void setProjection(sead::Projection* projection) {
        SetProjection(getPrimitiveDrawer(), projection);
    }
    void setCamera(sead::Camera* camera) {
        SetCamera(getPrimitiveDrawer(), camera);
    }
    void setDrawCtx(sead::DrawContext* draw_ctx) {
        SetDrawCtx(getPrimitiveDrawer(), draw_ctx);
    }

private:
    sead::Heap* mHeap = nullptr;
    CreateArg mCreateArg{};
    nn::os::MutexType mInitMutex;
    sead::FontBase** mDefaultFont = nullptr;
    sead::PrimitiveRenderer** mPrimitiveRenderer = nullptr;
    void (*mCreateDebugRenderer)(sead::Heap*, CreateArg&);
    int mGameVersion;
    bool mDrawDebug = true;
    bool mDrawUI2D = true;
    bool mDrawUI2DOverlay = true;
    bool mDrawTool2D = true;
    bool mDrawTool2DSuper = true;
    bool mDrawMain3D0 = true;
    bool mDrawUI3D0 = true;

    BeginFunc* Begin;
    EndFunc* End;
    DrawQuadFunc* DrawQuad;
    DrawQuad2Func* DrawQuad2;
    DrawBoxFunc* DrawBox;
    DrawWireCubeFunc* DrawWireCube;
    DrawLineFunc* DrawLine;
    DrawSphere4x8Func* DrawSphere4x8;
    DrawSphere8x16Func* DrawSphere8x16;
    DrawDisk32Func* DrawDisk32;
    DrawCircle32Func* DrawCircle32;
    DrawCylinder16Func* DrawCylinder16;
    DrawCylinder32Func* DrawCylinder32;
    SetModelMtxFunc* SetModelMtx;
    SetProjectionFunc* SetProjection;
    SetCameraFunc* SetCamera;
    SetDrawCtxFunc* SetDrawCtx;

    sead::Matrix34f mMatrix{1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f};
};