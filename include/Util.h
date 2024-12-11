#pragma once

#include "Game/LiveActor.h"

class LayoutActor;

/* utility functions that we need to implement */
namespace MR {
    bool tryStartDemoWithoutCinemaFrameValidStarPointer(LiveActor *, const char *);

    void setPaneScale(const LayoutActor *, f32, f32, const char *);
};