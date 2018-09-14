//
// Created by ilim on 2018/09/14.
//

#ifndef RENDERER_HIT_H
#define RENDERER_HIT_H
//#include "object.h"
struct  Sphere;

struct Hit {
    double distance;
    const Sphere* object;
};

#endif //RENDERER_HIT_H
