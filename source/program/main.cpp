#include "debugdraw.h"
#include "callbacks.h"
#include "utils.hpp"
#include "engineActor.h"
#include "offsets.h"

void* gCurrentScene = nullptr;

HOOK_DEFINE_INLINE(GetFormalScene) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        gCurrentScene = reinterpret_cast<void*>(ctx->X[0]);
    }
};

struct ResidentActor {
    ActorLink actorLink;
    void* watcher;
    sead::FixedSafeString<64> name;
};
static_assert(sizeof(ResidentActor) == 0x70);

struct ResidentActorMgr {
    char _00[0x20];
    sead::Buffer<ResidentActor> actors;

    ActorBase* getPlayer() const {
        int index = actors.size() - 1;
        ActorLink actor_link = actors[index].actorLink;
        return actor_link.getActor();
    }
};

using GetResidentActorMgr = ResidentActorMgr* (void*);
GetResidentActorMgr* getResidentActorMgr = nullptr;

void drawBoxBorders(const sead::BoundBox3f& bounds) {
    const sead::Vector3f max = bounds.getMax();
    const sead::Vector3f min = bounds.getMin();
    sead::Color4f color{0.9f, 0.5f, 0.7f, 1.f};
    // draw line seems to not work hmmm
    // I'll figure this out later
    gDrawMgr.drawLine(sead::Vector3f(min.x, min.y, min.z), sead::Vector3f(max.x, min.y, min.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, max.y, min.z), sead::Vector3f(max.x, max.y, min.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, max.y, max.z), sead::Vector3f(max.x, max.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, min.y, max.z), sead::Vector3f(max.x, min.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, min.y, min.z), sead::Vector3f(min.x, max.y, min.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, min.y, max.z), sead::Vector3f(min.x, max.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(max.x, min.y, max.z), sead::Vector3f(max.x, max.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(max.x, min.y, min.z), sead::Vector3f(max.x, max.y, min.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, min.y, min.z), sead::Vector3f(min.x, min.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(min.x, max.y, min.z), sead::Vector3f(min.x, max.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(max.x, max.y, min.z), sead::Vector3f(max.x, max.y, max.z), color, color);
    gDrawMgr.drawLine(sead::Vector3f(max.x, min.y, min.z), sead::Vector3f(max.x, min.y, max.z), color, color);
}

void drawMain3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .25f });
    writer.printf("Hi from Main 3D 0\n");

    if (gCurrentScene == nullptr) {
        writer.printf("Scene has not yet been initialized");
        return;
    }
    ResidentActorMgr* mgr = getResidentActorMgr(gCurrentScene);
    if (mgr == nullptr) {
        writer.printf("ResidentActorMgr has not yet been initialized");
        return;
    }
    ActorBase* player = mgr->getPlayer();
    if (player == nullptr) {
        writer.printf("Player actor is null");
        return;
    }

    sead::Matrix34f mtx = player->getTransform();
    const sead::BoundBox3f aabb = player->getAABB();

    float radius = aabb.getSizeX() > aabb.getSizeZ() ? aabb.getSizeX() * 0.5f : aabb.getSizeZ() * 0.5f;
    float height = aabb.getSizeY();
    sead::Color4f color{0.45f, 0.1f, 0.87f, 0.1f};

    gDrawMgr.setupRenderer(info);
    gDrawMgr.begin();
    gDrawMgr.setModelMtx(&mtx);
    drawBoxBorders(aabb);
    gDrawMgr.drawDisk32(sead::Vector3f(0.f, 0.f, 0.f), 2.f, color, color);
    gDrawMgr.drawCylinder32(sead::Vector3f(0.f, height * 0.5f, 0.f), radius, height, color, color);
    gDrawMgr.end();
}

extern "C" void exl_main(void* x0, void* x1) {
    initDebugDrawer();
    GetFormalScene::InstallAtOffset(sFormalSceneOffsets[gDrawMgr.version()]);
    getResidentActorMgr = reinterpret_cast<GetResidentActorMgr*>(exl::util::modules::GetTargetOffset(sGetResidentActorMgrOffsets[gDrawMgr.version()]));
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}