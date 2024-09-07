#include "lib.hpp"
#include "nn.hpp"
#include "utils.hpp"
#include "sead.h"
#include "binaryoffsethelper.h"
#include <cstring>

namespace agl::lyr {

struct Layer {
    char _whatever[0x89];
    u8 flag;
    char _whatever2[0x56];
    char* name;
};

} // namespace agl::lyr

// GraphicsModule CreateArg
struct CreateArg {
    char _whatever[0xb4c];
    int value0;
    char _whatever2[0x10];
    int value1;
};

void (*TextWriterPrintf)(sead::TextWriter*, const char*, ...);

void (*CreateDebugRenderers)(sead::Heap*, CreateArg&);

void** sDefaultFont;

sead::Heap* gStolenHeap;

CreateArg gCreateArg{};

static int sGameVersion = 5;

nn::os::MutexType gInitMutex;

HOOK_DEFINE_INLINE(StealHeap) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        gStolenHeap = reinterpret_cast<sead::Heap*>(ctx->X[sGameVersion == 0 ? 19 : 22]);
        // I want to initialize the font + renderer here but for some reason the text will not render if I do?
    }
};

HOOK_DEFINE_INLINE(GetCreateArg) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        gCreateArg.value0 = reinterpret_cast<CreateArg*>(ctx->X[1])->value0;
        gCreateArg.value1 = reinterpret_cast<CreateArg*>(ctx->X[1])->value1;
    }
};

HOOK_DEFINE_INLINE(EnableDebugDraw) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        char buf[0x110];
        if (*sDefaultFont == nullptr) {
            nn::os::LockMutex(&gInitMutex);
            if (*sDefaultFont == nullptr) {
                PRINT("Default font is null, attempting to initialize")
                CreateDebugRenderers(gStolenHeap, gCreateArg);
            }
            nn::os::UnlockMutex(&gInitMutex);
        }
        if (*sDefaultFont == nullptr) {
            PRINT("Failed to init default font")
            return;
        }
        if (strncmp("Tool 2D Super", reinterpret_cast<agl::lyr::Layer*>(ctx->X[21])->name, 0x100) == 0) {
            reinterpret_cast<agl::lyr::Layer*>(ctx->X[21])->flag |= (1 << 5);
            ctx->W[8] = 0x28;
        }
    }
};

HOOK_DEFINE_INLINE(FixFlag) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        ctx->W[8] = 1 << 5;
    }
};

HOOK_DEFINE_INLINE(TestPrint) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        sead::TextWriter* writer = reinterpret_cast<sead::TextWriter*>(ctx->X[21]);
        static sead::Vector2f cursor{ 0.f, 0.f };
        static sead::Color4f color{ .5f, .25f, 1.f, 1.f };
        // yes I know I should've just included the sead decomp to have proper assignment operators but I'm lazy :)
        writer->mCursor.x = cursor.x;
        writer->mCursor.y = cursor.y;
        writer->mColor.r = color.r;
        writer->mColor.g = color.g;
        writer->mColor.b = color.b;
        writer->mColor.a = color.a;
        TextWriterPrintf(writer, "Hello World");
        if (cursor.x < 100.f) {
            cursor.x += 3.f;
        } else {
            cursor.x -= 200.f;
        }
        if (cursor.y < 100.f) {
            cursor.y += 3.f;
        } else {
            cursor.y -= 200.f;
        }
        if (color.g < 1.f) {
            color.g += .1f;
        } else {
            color.g = .25f;
        }
    }
};

static constexpr u64 sTextWriterPrintfOffsets[6] = {
    0x0184c9e0, 0x018993e4, 0x01899004, 0x0188e2c8, 0x0187ab3c, 0x018890d4
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

static constexpr u64 sFixFlagOffsets[6] = {
    0x008589bc, 0x00840050, 0x00785a04, 0x00808f20, 0x007d2b08, 0x00819140
};

static constexpr u64 sTestPrintOffsets[6] = {
    0x02188cb0, 0x022081b0, 0x02207ed0, 0x021fd270, 0x021f38d0, 0x021fdd00
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking enviroment */
    exl::hook::Initialize();

    sGameVersion = InitializeAppVersion();
    if (sGameVersion == 0xffffffff || sGameVersion > 5) {
        char buf[0x20];
        PRINT("Error getting version");
        return;
    }
    char buf[0x8];
    PRINT("%u", sGameVersion);

    nn::os::InitializeMutex(&gInitMutex, true, 0);

    using PrintfFunc = void (sead::TextWriter*, const char*, ...);
    using CreateFunc = void (sead::Heap*, CreateArg&);
    TextWriterPrintf = reinterpret_cast<PrintfFunc*>(exl::util::modules::GetTargetOffset(sTextWriterPrintfOffsets[sGameVersion]));
    CreateDebugRenderers = reinterpret_cast<CreateFunc*>(exl::util::modules::GetTargetOffset(sCreateDebugRendererOffsets[sGameVersion]));

    sDefaultFont = reinterpret_cast<void**>(exl::util::modules::GetTargetOffset(sDefaultFontOffsets[sGameVersion]));

    StealHeap::InstallAtOffset(sStealHeapOffsets[sGameVersion]);
    GetCreateArg::InstallAtOffset(sGetCreateArgOffsets[sGameVersion]);
    EnableDebugDraw::InstallAtOffset(sEnableDebugDrawOffsets[sGameVersion]);
    // FixFlag::InstallAtOffset(sFixFlagOffsets[sGameVersion]);
    TestPrint::InstallAtOffset(sTestPrintOffsets[sGameVersion]);

    return;
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}