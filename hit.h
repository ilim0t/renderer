//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_HIT_H
#define RENDERER_HIT_H

#include "vector.h"
//#include "object.h"
struct BaseObject;

struct Hit {
    Vector3 point;
    Vector3 normal;
    const BaseObject *hit_object_ptr;
};

#endif //RENDERER_HIT_H
