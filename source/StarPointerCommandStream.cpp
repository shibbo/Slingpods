#include "StarPointerCommandStream.h"
#include "Game/LiveActor/Spine.h"
#include "Game/Screen/StarPointerDirector.h"
#include "Game/Util.h"
#include "Game/Util/StarPointerUtil.h"
#include "Util.h"
#include "JSystem/JMATrigonometric.h"

namespace NrvStarPointerCommandStream {
    NERVE_DECL_EXE(HostTypeNrvWait, StarPointerCommandStream, Wait);
    NERVE_DECL_EXE(HostTypeNrvSignal, StarPointerCommandStream, Signal);

    HostTypeNrvWait HostTypeNrvWait::sInstance;
    HostTypeNrvSignal HostTypeNrvSignal::sInstance;
};

namespace {
    static Color8 hOutColor(0xFF, 0, 0, 0xFF);
};

StarPointerCommandStream::StarPointerCommandStream(const TVec2f *pVec) : LayoutActor("スターポインタ指示線", true) {
    _20 = 0;
    _24 = 0;
    _28 = 0.0f;
    _2C = pVec;
    _30 = 0;
    _3C = -1;
    _40 = 0;
}

void StarPointerCommandStream::start(const TVec3f *pVec, bool a2) {
    _30 = pVec;
    _20 = a2;
    MR::showLayout(this);
    MR::startAnim(this, "Wait", 0);
    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, *_30);
    calcPose(screenPos,screenPos);
    calcAnim();
}

void StarPointerCommandStream::show() {
    MR::showLayout(this);
}

void StarPointerCommandStream::hide() {
    MR::setPaneScale(this, 0.0f, 0.0f, "PicRibbon");
    MR::hideLayout(this);
}

void StarPointerCommandStream::clear() {
    _30 = 0;
    _20 = 0;
}

void StarPointerCommandStream::init(const JMapInfoIter &rIter) {
    initLayoutManager("CommonStream", 1);
    MR::createAndAddPaneCtrl(this, "CommandStream", 1);
    MR::createAndAddPaneCtrl(this, "PicRibbon", 1);
    MR::createAndAddPaneCtrl(this, "PicArrow", 1);
    MR::createAndAddPaneCtrl(this, "Arrow", 1);
    MR::setFollowPos(&mFollowPos, this, "CommandStream");
    initNerve(&NrvStarPointerCommandStream::HostTypeNrvWait::sInstance);
    appear();
    MR::setPaneScale(this, 0.0f, 0.0f, "PicRibbon");
    MR::hideLayout(this);
}

void StarPointerCommandStream::control() {
    if (MR::isStarPointerInScreen(_3C)) {
        _24 = 0;
        _28 = _28 - 0.050000001f;
    }
    else {
        u32 v3 = _24 + 1;
        _24 = v3;
        if ((v3 & 0x10) != 0 && (v3 & 0xF) == 0) {
            MR::startSystemSE("SE_SY_DPD_DRAG_RANGE_QUIT", -1, -1);
        }
    }

    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, *_30);
    TVec2f v5;
    v5.x = _2C->x;
    v5.y = _2C->y;
    StarPointerFunction::forceInsideScreenEdge(&v5);
    calcPose(screenPos, v5);
}

void StarPointerCommandStream::calcPose(const TVec2f &rFollowPos, const TVec2f &a2) {
    mFollowPos.x = rFollowPos.x;
    mFollowPos.y = rFollowPos.y;
    TVec2f v5;
    v5.x = (a2.x - rFollowPos.x);
    v5.y = (a2.y - rFollowPos.y);

    // MR::isScreen16Per9 -- unused

    if (MR::isNearZero(v5, 0.001f)) {
        MR::setPaneScale(this, 0.0f, 0.0f, "PicRibbon");
    }
    else {
        f32 scalar;
        MR::separateScalarAndDirection(&scalar, &v5, v5);
        f32 lineWidth = calcLineWidth(scalar);
        f32 v8 = (scalar / 80.0f);
        scalar /= 80.0f;
        MR::setPaneScale(this, (lineWidth / 30.0f), v8, "PicRibbon");
        f32 tan = JMath::sAtanTable.atan2_(v5.x, v5.y);
        MR::setPaneRotate(this, 0.0f, 0.0f, (tan * 57.295776f), "Arrow");
    }
}

void StarPointerCommandStream::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }

    if (!MR::isStarPointerInScreen(_3C)) {
        setNerve(&NrvStarPointerCommandStream::HostTypeNrvSignal::sInstance);
    }
}

void StarPointerCommandStream::exeSignal() {
    // MR::isFirstStep() -- unused
    if (MR::isStarPointerInScreen(_3C)) {
        setNerve(&NrvStarPointerCommandStream::HostTypeNrvWait::sInstance);
    }
}

f32 StarPointerCommandStream::calcLineWidth(f32 scalar) const {
    f32 norm = MR::normalize(scalar, 40.0f, 400.0f);
    return ((6.0f * norm) + (32.0f * (1.0f - norm)));
}

StarPointerCommandStream::~StarPointerCommandStream() {
    
}