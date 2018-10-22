//
// Created by ilim on 2018/10/21.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "vector.h"
#include "random.h"
#include "ray.h"

struct MaterialBase {
    Vector3 reflectance;

    MaterialBase(const Vector3 &reflectance) : reflectance(reflectance) {}

    virtual std::tuple<Ray, double>
    reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const = 0;

    virtual Vector3 f(const Vector3 &point, const Vector3 &in_direction, const Vector3 &out_direction) const = 0;
};

struct Diffuse : public MaterialBase {
    Diffuse(const Vector3 &reflectance) : MaterialBase(reflectance) {}

    std::tuple<Ray, double> reflect(const Vector3 &point, const Vector3 &, const Vector3 &normal) const {
        double theta = 0.5 * std::acos(1 - 2 * random_::rand());
        double phi = random_::rand() * 2 * M_PI;

        double pdf = std::cos(theta) / M_PI;

        double x = std::cos(phi) * std::sin(theta);
        double y = std::sin(phi) * std::sin(theta);
        double z = std::cos(theta);

        auto const&[u, v] = vector3::tangent_space(normal);

        Ray ray{point + normal * 0.001, x * u + y * v + z * normal, -1};

        return {ray, pdf};
    }

    Vector3 f(const Vector3 &, const Vector3 &, const Vector3 &) const {
        return reflectance / M_PI;
    };

};

struct Mirror : public MaterialBase {
    Mirror(const Vector3 &reflectance = Vector3(1)) : MaterialBase(reflectance) {}

    std::tuple<Ray, double> reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const {
        Ray ray{point + normal * 0.001,
                in_direction.unit() - vector3::dot(normal.unit(), in_direction.unit()) * normal.unit() * 2,
                -1};
        return {ray, 1};
    }

    Vector3 f(const Vector3 &, const Vector3 &, const Vector3 &) const {
        return reflectance;
    };


};

#endif //RENDERER_MATERIAL_H
