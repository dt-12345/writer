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

int gColorIndex = 0;
static const sead::Color4f sRainbowGradientColors[] = {
    sead::Color4f{1.0f, 0.0f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.043137254901960784f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.08627450980392157f, 0.0f, 0.1f}, 
    sead::Color4f{1.0f, 0.13333333333333333f, 0.0f, 0.1f}, 
    sead::Color4f{1.0f, 0.17647058823529413f, 0.0f, 0.1f}, 
    sead::Color4f{1.0f, 0.2235294117647059f, 0.0f, 0.1f},  
    sead::Color4f{1.0f, 0.26666666666666666f, 0.0f, 0.1f}, 
    sead::Color4f{1.0f, 0.3137254901960784f, 0.0f, 0.1f},  
    sead::Color4f{1.0f, 0.3568627450980392f, 0.0f, 0.1f},  
    sead::Color4f{1.0f, 0.403921568627451f, 0.0f, 0.1f},   
    sead::Color4f{1.0f, 0.4470588235294118f, 0.0f, 0.1f},  
    sead::Color4f{1.0f, 0.49411764705882355f, 0.0f, 0.1f}, 
    sead::Color4f{1.0f, 0.5372549019607843f, 0.0f, 0.1f},  
    sead::Color4f{1.0f, 0.5803921568627451f, 0.0f, 0.1f},  
    sead::Color4f{1.0f, 0.6274509803921569f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.6705882352941176f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.7176470588235294f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.7607843137254902f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.807843137254902f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.8509803921568627f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.8980392156862745f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.9411764705882353f, 0.0f, 0.1f},
    sead::Color4f{1.0f, 0.9882352941176471f, 0.0f, 0.1f},
    sead::Color4f{0.9647058823529412f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.9176470588235294f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.8745098039215686f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.8313725490196079f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.7843137254901961f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.7411764705882353f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.6941176470588235f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.6509803921568628f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.6039215686274509f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.5607843137254902f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.5137254901960784f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.47058823529411764f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.4235294117647059f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.3803921568627451f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.3333333333333333f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.2901960784313726f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.24705882352941178f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.2f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.1568627450980392f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.10980392156862745f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.06666666666666667f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.0196078431372549f, 1.0f, 0.0f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.0196078431372549f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.06666666666666667f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.10980392156862745f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.1568627450980392f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.2f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.24705882352941178f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.2901960784313726f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.3333333333333333f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.3803921568627451f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.4235294117647059f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.47058823529411764f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.5137254901960784f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.5607843137254902f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.6039215686274509f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.6509803921568628f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.6941176470588235f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.7411764705882353f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.7843137254901961f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.8313725490196079f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.8745098039215686f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.9176470588235294f, 0.1f},
    sead::Color4f{0.0f, 1.0f, 0.9647058823529412f, 0.1f},
    sead::Color4f{0.0f, 0.9882352941176471f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.9411764705882353f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.8980392156862745f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.8509803921568627f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.807843137254902f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.7607843137254902f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.7176470588235294f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.6705882352941176f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.6274509803921569f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.5803921568627451f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.5372549019607843f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.49411764705882355f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.4470588235294118f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.403921568627451f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.3568627450980392f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.3137254901960784f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.26666666666666666f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.2235294117647059f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.17647058823529413f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.13333333333333333f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.08627450980392157f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.043137254901960784f, 1.0f, 0.1f},
    sead::Color4f{0.0f, 0.0f, 1.0f, 0.1f},
};

sead::Color4f getRainbowColor() {
    sead::Color4f color = sRainbowGradientColors[gColorIndex];
    gColorIndex = (gColorIndex % 90) + 1;
    return color;
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

    sead::Matrix33f rotation;
    rotation.setMul(player->rot, sead::Matrix33f(1.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f));
    sead::Matrix34f mtx{rotation, player->pos};
    const sead::Color4f color = getRainbowColor();

    const float length = 0.5f;
    const float radius = 0.1f;

    gDrawMgr.setupRenderer(info);
    gDrawMgr.begin();
    gDrawMgr.setModelMtx(&mtx);
    const sead::Color4f actorPosColor = sead::Color4f(1.0f, 0.f, 0.f, 0.1f);
    gDrawMgr.drawSphere8x16(sead::Vector3f(0.f, 0.f, 0.f), radius, actorPosColor, actorPosColor);
    auto physCmp = player->getPhysicsComponent();
    if (physCmp && physCmp->controllerSet && physCmp->controllerSet->mainRigidBody) {
        gDrawMgr.setModelMtx(&physCmp->controllerSet->mainRigidBody->lastTransform);
        const sead::Color4f rigidBodyPosColor = sead::Color4f(0.0f, 1.f, 0.f, 0.1f);
        const sead::Color4f rigidBodyPrevPosColor = sead::Color4f(0.0f, 1.f, 1.f, 0.1f);
        const sead::Color4f rigidBodyNextPosColor = sead::Color4f(1.0f, 1.f, 0.f, 0.1f);
        gDrawMgr.drawSphere8x16(sead::Vector3f(0.f, 0.f, 0.f), radius + 0.1f, rigidBodyPosColor, rigidBodyPosColor);
        gDrawMgr.setModelMtx(&physCmp->controllerSet->mainRigidBody->prevTransform);
        gDrawMgr.drawSphere8x16(sead::Vector3f(0.f, 0.f, 0.f), radius + 0.15f, rigidBodyPrevPosColor, rigidBodyPrevPosColor);
        sead::Matrix34f nextTransform;
        if (physCmp->controllerSet->mainRigidBody->changeRequest && (physCmp->controllerSet->mainRigidBody->changeRequest->flags >> 6 & 1) != 0) {
            nextTransform = physCmp->controllerSet->mainRigidBody->changeRequest->nextTransform;
        } else {
            nextTransform = physCmp->controllerSet->mainRigidBody->lastTransform;
        }
        gDrawMgr.setModelMtx(&nextTransform);
        gDrawMgr.drawSphere8x16(sead::Vector3f(0.f, 0.f, 0.f), radius + 0.2f, rigidBodyNextPosColor, rigidBodyNextPosColor);

    } else {
        char buf[0x10];
        PRINT("phys null")
    }
    auto modelCmp = player->getModelComponent();
    if (modelCmp && modelCmp->model) {
        gDrawMgr.setModelMtx(&modelCmp->model->modelMtx);
        const sead::Color4f modelPosColor = sead::Color4f(0.0f, 0.f, 1.f, 0.1f);
        gDrawMgr.drawSphere8x16(sead::Vector3f(0.f, 0.f, 0.f), radius + 0.3f, modelPosColor, modelPosColor);
    } else {
        char buf[0x10];
        PRINT("model null")
    }
    // gDrawMgr.drawCylinder32(sead::Vector3f(0.f, length * 0.5f + 0.2f, -0.7f), radius, length, color, color);
    // gDrawMgr.drawSphere8x16(sead::Vector3f(-radius, 0.2f, -0.7f), radius, color, color);
    // gDrawMgr.drawSphere8x16(sead::Vector3f(radius, 0.2f, -0.7f), radius, color, color);
    // gDrawMgr.drawSphere8x16(sead::Vector3f(0.f, length + 0.2f, -0.7f), radius, color, color);
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