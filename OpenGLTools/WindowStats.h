#ifndef WINDOW_STATS_H
#define WINDOW_STATS_H

#include <cameraBaseClass.h>

struct WindowStats
{
    // Width X, height Y, Viewport X, Viewport Y
    float wx;
    float wy;

    float vx;
    float vy;
};

struct WindowContext
{
    WindowStats windowDimensions;
    cameraBaseClass*      activeCamera;
};



#endif