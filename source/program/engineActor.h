#pragma once

#include "types.h"
#include <math/seadVector.h>
#include <math/seadMatrix.h>
#include <math/seadBoundBox.h>

struct ActorBase {
    char _00[0x2b4];
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