#include "binaryoffsethelper.h"
#include "debugdraw.h"
#include "callbacks.h"
#include "offsets.h"

#include "common/aglDrawContext.h"

PrintfFunc* TextWriterPrintf;

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
    TextWriterPrintf = reinterpret_cast<PrintfFunc*>(exl::util::modules::GetTargetOffset(sTextWriterPrintfOffsets[gDrawMgr.version()]));
    TextWriterCtor = reinterpret_cast<Ctor*>(exl::util::modules::GetTargetOffset(sTextWriterCtorOffsets[gDrawMgr.version()]));
    TextWriterSetupGraphics = reinterpret_cast<SetupGraphics*>(exl::util::modules::GetTargetOffset(sTextWriterSetupGraphicsOffsets[gDrawMgr.version()]));
    gDrawMgr.setCreateCallback(reinterpret_cast<CreateFunc*>(exl::util::modules::GetTargetOffset(sCreateDebugRendererOffsets[gDrawMgr.version()])));
    gDrawMgr.setFont(reinterpret_cast<sead::FontBase**>(exl::util::modules::GetTargetOffset(sDefaultFontOffsets[gDrawMgr.version()])));

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