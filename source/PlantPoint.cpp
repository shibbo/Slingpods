#include "PlantPoint.h"
#include "Game/Util.h"

PlantPoint::PlantPoint(const TVec3f &rPos, const TVec3f &rYDir, f32 a3) {
    mPosition.set(rPos);
    mYDir.set(rYDir);
    mXDir.set(1.0f, 0.0f, 0.0f);
    mZDir.set(0.0f, 0.0f, 1.0f);
    _34 = a3;

    TVec3f cross;
    PSVECCrossProduct(mXDir, mYDir, cross);

    if (MR::isNearZero(cross, 0.001f)) {
        MR::makeAxisUpFront(&mXDir, &mZDir, mYDir, mZDir);
    }
    else {
        MR::makeAxisUpSide(&mZDir, &mXDir, mYDir, mXDir);
    }
}

void PlantPoint::setAxisAndPos(const TVec3f &rXDir, const TVec3f &rYDir, const TVec3f &rZDir, const TVec3f &rPos) {
    mXDir.set(rXDir);
    mYDir.set(rYDir);
    mZDir.set(rZDir);
    mPosition.set(rPos);
}