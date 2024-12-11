#pragma once

#include "Game/Screen/LayoutActor.h"

class StarPointerCommandStream : public LayoutActor {
public:
    StarPointerCommandStream(const TVec2f *);

    virtual ~StarPointerCommandStream();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void start(const TVec3f *, bool);
    void show();
    void hide();
    void clear();
    void calcPose(const TVec2f &, const TVec2f &);
    void exeWait();
    void exeSignal();
    f32 calcLineWidth(f32) const;

    u8 _20;
    u32 _24;
    f32 _28;
    const TVec2f* _2C;
    const TVec3f* _30;
    TVec2f mFollowPos;      // _34
    s32 _3C;
    u32 _40;
};