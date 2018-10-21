//
// Created by ilim on 2018/10/21.
//

#ifndef RENDERER_SURFACE_H
#define RENDERER_SURFACE_H

#include "vector.h"
#include "random.h"

struct SurfaceBase {
    virtual Vector3 reflect(const Vector3& point, const Vector3& in_direction, const Vector3& normal) const = 0;
};

struct Diffuse : public SurfaceBase {
    Vector3 reflect(const Vector3& point, const Vector3& in_direction, const Vector3& normal) const {
        auto [u,v] = vector3::tangent_space(normal);
        double r = random_::rand();
        double theta = random_::rand() * 2. * M_PI;

        double x = r * std::cos(theta);
        double y = r * std::sin(theta);

        return x*u + y*v + std::sqrt(1-x*x-y*y) * normal;
    }
};

struct Mirror : public SurfaceBase {
    Vector3 reflect(const Vector3& point, const Vector3& in_direction, const Vector3& normal) const {
        return in_direction.unit() - vector3::dot(normal.unit(), in_direction.unit()) * normal.unit() * 2;
    }

};
#endif //RENDERER_SURFACE_H
