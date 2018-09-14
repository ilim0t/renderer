//
// Created by ilim0t on 2018/09/13.
//

#ifndef RENDERER_RAY_H
#define RENDERER_RAY_H
#include "vector.h"

struct Ray {
    Vector origin;
    Vector direction;

    Ray(const Vector& origin, const Vector& direction) : origin(origin), direction(normalize(direction)) {};
};

#endif //RENDERER_RAY_H
