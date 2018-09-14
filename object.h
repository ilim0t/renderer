//
// Created by ilim on 2018/09/14.
//

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H
#include "ray.h"
#include "hit.h"
#include "vector.h"
#include <optional>
#include <cmath>

struct Sphere {
    Vector position;
    double radius;

    std::optional<Hit> intersect(const Ray& ray, double dmax, double dmin) const {
        const Vector relative_position =  position - ray.origin;
        const double b = dot(relative_position, ray.direction);
        const double distance2 = dot(relative_position, relative_position) - b * b;
        if (distance2 > radius * radius) {
            return {};
        }

        const double t1 = b - std::sqrt(radius * radius - distance2);
        if (dmin < t1 && t1 < dmax) {
            return Hit{t1, this};
        }
        const double t2 = b + std::sqrt(radius * radius - distance2);
        if (dmin < t2 && t2 < dmax) {
            return Hit{t2, this};
        }
        return {};
    }
};

#endif //RENDERER_OBJECT_H
