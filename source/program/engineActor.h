#pragma once

#include "types.h"
#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <math/seadMatrix.h>
#include <math/seadBoundBox.h>
#include <prim/seadSafeString.h>

struct ChangeRequest {
    char _00[0x8];
    sead::Matrix34f nextTransform;
    char _38[0x9c];
    u32 flags;
};

struct RigidBody {
    char _00[0x60];
    ChangeRequest* changeRequest;
    char _68[0x30];
    sead::Matrix34f lastTransform;
    sead::Matrix34f prevTransform;
};
static_assert(offsetof(RigidBody, lastTransform) == 0x98);

struct ControllerSet {
    char _00[0x150];
    RigidBody* mainRigidBody;
};

struct Component {
    void* __vtbl;
    sead::SafeString refPath;
};

struct PhysicsComponent : public Component {
    char _10[0x10];
    ControllerSet* controllerSet;
};
static_assert(offsetof(PhysicsComponent, controllerSet) == 0x20);

struct Model {
    char _00[0x1f8];
    sead::Matrix34f modelMtx;
};

struct ModelComponent : public Component {
    char _10[0x18];
    Model* model;
};
static_assert(offsetof(ModelComponent, model) == 0x28);

struct ActorBase {
    char _00[0x220];
    sead::PtrArray<Component> components;
    char _0230[0x84];
    sead::Vector3f pos;
    sead::Matrix33f rot;
    sead::Vector3f scale;
    sead::Vector3f nextPos;
    sead::Matrix33f nextRot;
    sead::Vector3f lastLinearVel;
    sead::Vector3f lastAngularVel;
    float _338[6];
    sead::BoundBox3f aabb;

    sead::Matrix34f getTransform() const {
        return sead::Matrix34f(rot, pos);
    }

    PhysicsComponent* getPhysicsComponent() const {
        return reinterpret_cast<PhysicsComponent*>(components[0xa]);
    }

    ModelComponent* getModelComponent() const {
        return reinterpret_cast<ModelComponent*>(components[2]);
    }

    const sead::BoundBox3f& getAABB() const { return aabb; } 
};
static_assert(offsetof(ActorBase, aabb) == 0x350);

struct BaseProcLinkData {
    char readCS[0x20];
    char writeCS[0x20];
    ActorBase* baseProc;
    int baseProcId;
    u64 refMask;
    u8 _58;
};
static_assert(sizeof(BaseProcLinkData) == 0x60);

struct ActorLink {
    void* __vtbl;
    BaseProcLinkData* linkData;
    int baseProcId;
    bool _14;
    u8 _15;
    u8 refType;

    bool isValid() const { return linkData != nullptr && linkData->baseProc != nullptr; }
    ActorBase* getActor() const {
        if (!isValid()) return nullptr;
        return linkData->baseProc;
    }
};
static_assert(sizeof(ActorLink) == 0x18);