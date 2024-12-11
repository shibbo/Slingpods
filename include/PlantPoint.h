#pragma once

#include "JSystem/JGeometry.h"

class PlantPoint {
public:
    PlantPoint(const TVec3f &, const TVec3f &, f32);

    virtual void setAxisAndPos(const TVec3f &, const TVec3f &, const TVec3f &, const TVec3f &);

    TVec3f mPosition;       // 0x4
    TVec3f mXDir;           // 0x10
    TVec3f mYDir;           // 0x1C
    TVec3f mZDir;           // 0x28
    f32 _34;
};