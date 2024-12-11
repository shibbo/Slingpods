#include "SpaceCocoon.h"
#include "Demo.h"
#include "Game/Util.h"
#include "Game/LiveActor/Spine.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Camera/CameraTargetArg.h"
#include "Game/Util/CSSoundUtil.h"

namespace NrvSpaceCocoon {
    NERVE_DECL_EXE(SpaceCocoonNrvStop, SpaceCocoon, Stop);
    NERVE_DECL_EXE(SpaceCocoonNrvFree, SpaceCocoon, Free);
    NERVE_DECL_EXE(SpaceCocoonNrvFreeInvalid, SpaceCocoon, FreeInvalid);
    NERVE_DECL_EXE(SpaceCocoonNrvKinopioLand, SpaceCocoon, BindLand);
    NERVE_DECL_EXE(SpaceCocoonNrvKinopioWait, SpaceCocoon, BindWait);
    NERVE_DECL_EXE(SpaceCocoonNrvWaitKinopioAimDemo, SpaceCocoon, WaitKinopioAimDemo);
    NERVE_DECL_EXE(SpaceCocoonNrvKinopioAim, SpaceCocoon, BindAim);
    NERVE_DECL_EXE(SpaceCocoonNrvKinopioAttack, SpaceCocoon, BindAttack);
    NERVE_DECL_EXE(SpaceCocoonNrvKinopioAttackSuccess, SpaceCocoon, BindAttackSuccess);
    NERVE_DECL_EXE(SpaceCocoonNrvBindLand, SpaceCocoon, BindLand);
    NERVE_DECL_EXE(SpaceCocoonNrvBindWait, SpaceCocoon, BindWait);
    NERVE_DECL_EXE(SpaceCocoonNrvBindAim, SpaceCocoon, BindAim);
    NERVE_DECL_EXE(SpaceCocoonNrvBindAttack, SpaceCocoon, BindAttack);
    NERVE_DECL_EXE(SpaceCocoonNrvBindAttackSuccess, SpaceCocoon, BindAttackSuccess);
};

namespace {
    static Color8 sColor(0xC8, 0xC8, 0xD7, 0xFF);
};

SpaceCocoon::SpaceCocoon(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90.set(0.0f, 0.0f, 0.0f);
    _9C.set(0.0f, 0.0f, 0.0f);
    _A8.set(1.0f, 0.0f, 0.0f);
    _B4.set(0.0f, 1.0f, 0.0f);
    _C0.set(0.0f, 0.0f, 1.0f);
    _CC = 100.0f;
    _D0.set(0.0f, 0.0f, 0.0f);
    _DC = -1;
    _E0.set(0.0f, 0.0f, 0.0f);
    _EC.set(0.0f, 0.0f, 0.0f);
    _F8 = 60;
    _FC = 0;
    _100 = 0;
    _134 = 0;
    mPlantPoints = 0;
    mTargetMtx = 0;
    mCameraInf = 0;
    mCocoonModel = 0;
    mTex = 0;
    _104.identity();
    _148.identity();
}

// SpaceCocoon::init

void SpaceCocoon::exeStop() {
    if (MR::isFirstStep(this)) {
        _FC = 0;   
    }

    tryTouch();
}

void SpaceCocoon::exeFree() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "Bind");
    }

    tryTouch();

    if (updateSpringPoint()) {
        _EC.zero();
        _E0.set(_90);
        mTranslation.set(_90);
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvStop::sInstance);
    }
    else {
        mTranslation.set(_E0);
    }
}

void SpaceCocoon::exeFreeInvalid() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
    }

    updateSpringPoint();
    mTranslation.set(_E0);

    if (MR::isGreaterStep(this, 5) && (PSVECDistance(_100->mTranslation, mTranslation) > 300.0f || (MR::isOnGroundPlayer()))) {
        _100 = 0;
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvFree::sInstance);
    }
}

void SpaceCocoon::exeBindLand() {
    if (MR::isFirstStep(this)) {
        if (isKinopioAttached()) {
            MR::startBckWithInterpole(_100, "CocoonLand", 1);
            setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioWait::sInstance);
            return;
        }

        MR::startBckPlayer("CocoonLand", NULL);
        CameraTargetArg arg(mTargetMtx);
        MR::startMultiActorCameraTargetOther(this, mCameraInf, "ウェイト", arg, -1);
        MR::startSound(_100, "SE_PV_CATCH", -1, -1);
        MR::startSound(this, "SE_OJ_SPACE_COCOON_ATTACH", -1, -1);
    }

    if (isKinopioAttached() || !updateBindWait() && (tryTouch(), MR::isBckStopped(_100))) {
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvBindWait::sInstance);
    }
}

void SpaceCocoon::exeBindWait() {
    if (MR::isFirstStep(this)) {
        if (isKinopioAttached()) {
            MR::startBck(_100, "CocoonWait", NULL);
            MR::validateClipping(this);
            MR::validateClipping(_100);
            MR::sendSimpleMsgToActor(210, _100);
        }
        else {
            MR::startBckPlayer("CocoonWait", NULL);
        }

        mVelocity.zero();
    }

    if (!updateBindWait()) {
        if (tryTouch()) {
            if (!isKinopioAttached()) {
                MR::startBckPlayer("CocoonReaction", NULL);
            }
        }
        else if (!isKinopioAttached() && MR::isBckOneTimeAndStopped(_100)) {
            MR::startBckPlayer("CocoonWait", NULL);
        }
    }
}

void SpaceCocoon::exeWaitKinopioAimDemo() {
    if (MR::tryStartDemoWithoutCinemaFrameValidStarPointer(this, "キノピオ狙い中")) {
        MR::startMultiActorCameraTargetPlayer(this, mCameraInf, "狙い中", -1);
        setNerve(&NrvSpaceCocoon::SpaceCocoonNrvKinopioAim::sInstance);
    }
}

void SpaceCocoon::exeBindAim() {
    if (MR::isFirstStep(this)) {
        if (isKinopioAttached()) {
            MR::sendSimpleMsgToActor(211, _100);
        }

        MR::startSound(this, "SE_OJ_SPACE_COCOON_DRAG_ST", -1, -1);

        if (_DC == 1) {
            MR::startCSSound2P("CS_DPD_HIT", 0, 0, 1.0f);
        }
    }

    f32 dist = PSVECDistance(mTranslation, _90);
    if (dist >= 100.0f) {
        MR::startLevelSound(this, "SE_OJ_LV_SPACE_COCOON_DRAG", (100.0f * ((dist - 100.0f) / 400.0f)), -1, -1);
    }

    if (!tryRelease()) {
        updateHang();
    }
}

void SpaceCocoon::exeBindAttack() {
    if (MR::isFirstStep(this)) {
        if (isKinopioAttached()) {
            MR::startBck(_100, "CocoonFly", NULL);
            MR::invalidateClipping(this);
            MR::invalidateClipping(_100);
            MR::startSound(_100, "SE_SV_KINOPIO_TALK_GLAD_FLY", -1, -1);
        }
        else {
            MR::startBckPlayer("CocoonFly", NULL);
            MR::startSound(_100, "SE_PV_JUMP_JOY", -1, -1);
        }

        MR::startSound(this, "SE_OJ_SPACE_COCOON_LAUNCH", -1, -1);
        MR::onBind(this);
    }

    updateBindAttack();

    if (tryAttackMap() || MR::isStep(this, _F8)) {
        MR::endMultiActorCamera(this, mCameraInf, "攻撃中", true, -1);

        if (MR::isStep(this, _F8)) {
            if (!isKinopioAttached()) {
                MR::startBckPlayer("AirRotation", NULL);
            }
        }

        endBind(TVec3f(0.0f, 0.0f, 0.0f), false);
    }
}