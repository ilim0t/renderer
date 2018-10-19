//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_RAY_H
#define RENDERER_RAY_H

#include "vector.h"
struct Ray {
    Vector3 origin;
    Vector3 direction;
    int depth;
};
#endif //RENDERER_RAY_H
