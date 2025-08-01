#include "binaryoffsethelper.h"
#include "debugdraw.h"
#include "callbacks.h"
#include "offsets.h"

#include "common/aglDrawContext.h"

PrintfFunc* TextWriterPrintf;
DebugDrawMgr gDrawMgr;

using GetHeap = sead::Heap* (bool thread_safe);
GetHeap* getVirtualAddressHeap = nullptr;

HOOK_DEFINE_INLINE(SetDebugHeap) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        ctx->X[0] = reinterpret_cast<u64>(getVirtualAddressHeap(true));
    }
};

HOOK_DEFINE_INLINE(EnableDebugDraw) {
    static void Callback(exl::hook::InlineCtx* ctx) {
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

    using Ctor = void (sead::TextWriter*, sead::DrawContext*, sead::Viewport*);
    using SetupGraphics = void (sead::DrawContext*);

    #define OFFSET(offsets) exl::util::modules::GetTargetOffset((offsets)[gDrawMgr.version()])
    getVirtualAddressHeap = reinterpret_cast<GetHeap*>(OFFSET(sGetVirtualAddressHeapOffsets));
    TextWriterPrintf = reinterpret_cast<PrintfFunc*>(OFFSET(sTextWriterPrintfOffsets));
    TextWriterCtor = reinterpret_cast<Ctor*>(OFFSET(sTextWriterCtorOffsets));
    TextWriterSetupGraphics = reinterpret_cast<SetupGraphics*>(OFFSET(sTextWriterSetupGraphicsOffsets));
    gDrawMgr.setPrimitiveRenderer(reinterpret_cast<sead::PrimitiveRenderer**>(OFFSET(sPrimitiveRendererOffsets)));
    #undef OFFSET

    EnableDebugDraw::InstallAtOffset(sEnableDebugDrawOffsets[gDrawMgr.version()]);
    DebugDraw::InstallAtOffset(sDebugDrawOffsets[gDrawMgr.version()]);

    SetDebugHeap::InstallAtOffset(sSetDebugHeapOffsets[gDrawMgr.version()]);

    PRINT("Initialized");
    
    return;
}

sead::TextWriter initializeTextWriter(const agl::lyr::RenderInfo& info) {
    TextWriterSetupGraphics(info.draw_ctx);
    sead::TextWriter writer;
    TextWriterCtor(&writer, dynamic_cast<sead::DrawContext*>(info.draw_ctx), info.viewport);
    return writer;
}