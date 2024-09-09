#include "binaryoffsethelper.h"
#include "debugdraw.h"
#include "callbacks.h"
#include "offsets.h"

#include "common/aglDrawContext.h"

PrintfFunc* TextWriterPrintf;
DebugDrawMgr gDrawMgr;

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

HOOK_DEFINE_INLINE(StealHeap) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        gDrawMgr.setHeap(reinterpret_cast<sead::Heap*>(ctx->X[gDrawMgr.version() == 0 ? 19 : 22]));
        // I want to initialize the font + renderer here but for some reason the text will not render if I do?
    }
};

HOOK_DEFINE_INLINE(GetCreateArg) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        gDrawMgr.initCreateArg(reinterpret_cast<CreateArg*>(ctx->X[1])->value0, reinterpret_cast<CreateArg*>(ctx->X[1])->value1);
    }
};

HOOK_DEFINE_INLINE(EnableDebugDraw) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        if (!gDrawMgr.createDebugRenderer()) {
            char buf[0x20];
            PRINT("Failed to init default font")
            return;
        }
        if (gDrawMgr.isDrawDebug()) {
            ctx->W[8] = 0x28;
        } else {
            ctx->W[8] = 0;
        }
    }
};

HOOK_DEFINE_REPLACE(DebugDraw) {
    static void Callback(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
        if (strncmp("UI 2D", layer->getName(), 0x100) == 0 && gDrawMgr.isDrawUI2D()) drawUI2D(layer, info);
        if (strncmp("UI_2D_Overlay", layer->getName(), 0x100) == 0 && gDrawMgr.isDrawUI2DOverlay()) drawUI2DOverlay(layer, info);
        if (strncmp("Tool 2D", layer->getName(), 0x100) == 0 && gDrawMgr.isDrawTool2D()) drawTool2D(layer, info);
        if (strncmp("Tool 2D Super", layer->getName(), 0x100) == 0 && gDrawMgr.isDrawTool2DSuper()) drawTool2DSuper(layer, info);
        if (strncmp("Main_3D_0", layer->getName(), 0x100) == 0 && gDrawMgr.isDrawMain3D0()) drawMain3D0(layer, info);
        if (strncmp("UI_3D_0", layer->getName(), 0x100) == 0 && gDrawMgr.isDrawUI3D0()) drawUI3D0(layer, info);
    }
};

void (*TextWriterCtor)(sead::TextWriter*, sead::DrawContext*, sead::Viewport*);
void (*TextWriterSetupGraphics)(sead::DrawContext*);

void initDebugDrawer() {
    /* Setup hooking enviroment */
    exl::hook::Initialize();

    char buf[0x100];

    PRINT("Initializing");
    gDrawMgr.initialize(InitializeAppVersion());
    if (gDrawMgr.version() == 0xffffffff || gDrawMgr.version() > 5) {
        PRINT("Error getting version");
        return;
    }
    PRINT("%u", gDrawMgr.version());

    using CreateFunc = void (sead::Heap*, CreateArg&);
    using Ctor = void (sead::TextWriter*, sead::DrawContext*, sead::Viewport*);
    using SetupGraphics = void (sead::DrawContext*);
    TextWriterPrintf = reinterpret_cast<PrintfFunc*>(OFFSET(sTextWriterPrintfOffsets));
    TextWriterCtor = reinterpret_cast<Ctor*>(OFFSET(sTextWriterCtorOffsets));
    TextWriterSetupGraphics = reinterpret_cast<SetupGraphics*>(OFFSET(sTextWriterSetupGraphicsOffsets));
    gDrawMgr.setCreateCallback(reinterpret_cast<CreateFunc*>(OFFSET(sCreateDebugRendererOffsets)));
    gDrawMgr.setFont(reinterpret_cast<sead::FontBase**>(OFFSET(sDefaultFontOffsets)));
    gDrawMgr.setPrimitiveRenderer(reinterpret_cast<sead::PrimitiveRenderer**>(OFFSET(sPrimitiveRendererOffsets)));

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

    StealHeap::InstallAtOffset(sStealHeapOffsets[gDrawMgr.version()]);
    GetCreateArg::InstallAtOffset(sGetCreateArgOffsets[gDrawMgr.version()]);
    EnableDebugDraw::InstallAtOffset(sEnableDebugDrawOffsets[gDrawMgr.version()]);
    DebugDraw::InstallAtOffset(sDebugDrawOffsets[gDrawMgr.version()]);

    PRINT("Initialized");
    
    return;
}

sead::TextWriter initializeTextWriter(const agl::lyr::RenderInfo& info) {
    TextWriterSetupGraphics(info.draw_ctx);
    sead::TextWriter writer;
    TextWriterCtor(&writer, dynamic_cast<sead::DrawContext*>(info.draw_ctx), info.viewport);
    return writer;
}