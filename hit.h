//
// Created by ilim0t on 2018/09/14.
//

#ifndef RENDERER_HIT_H
#define RENDERER_HIT_H
//#include "object.h"
#include "vector.h"
struct  Sphere;

struct Hit {
    double distance;
    Vector point;
    Vector normal;
    const Sphere* object;

    Hit(double distance, const Sphere* object) : distance(distance), object(object) {}
};

#endif //RENDERER_HIT_H
