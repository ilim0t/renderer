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

    virtual std::tuple<Ray, Vector3>
    reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const = 0;
};

struct Diffuse : public MaterialBase {
    Diffuse(const Vector3 &reflectance) : MaterialBase(reflectance) {}

    std::tuple<Ray, Vector3> reflect(const Vector3 &point, const Vector3 &, const Vector3 &normal) const {
        double theta = 0.5 * std::acos(1 - 2 * random_::rand());
        double phi = random_::rand() * 2 * M_PI;

        double x = std::cos(phi) * std::sin(theta);
        double y = std::sin(phi) * std::sin(theta);
        double z = std::cos(theta);

        auto const&[u, v] = vector3::tangent_space(normal);

        Ray next_ray{point + normal * 0.001, x * u + y * v + z * normal, -1};
        double cos = std::max(vector3::dot(normal.unit(), next_ray.direction.unit()), 0.);
        double pdf = std::cos(theta) / M_PI;

        return {next_ray, reflectance / M_PI * cos / pdf};
    }
};

struct Mirror : public MaterialBase {
    Mirror(const Vector3 &reflectance = Vector3(1)) : MaterialBase(reflectance) {}

    std::tuple<Ray, Vector3>
    reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const {
        Ray next_ray{point + normal * 0.001,
                     in_direction.unit() - vector3::dot(normal.unit(), in_direction.unit()) * normal.unit() * 2, -1};
        double cos = std::max(vector3::dot(normal.unit(), next_ray.direction.unit()), 0.);

        return {next_ray, reflectance * cos};
    }
};

//struct Fuzz : public MaterialBase {
//    double half_vertex_angle;
//
//    Fuzz(double half_vertex_angle, const Vector3 &reflectance = Vector3(1)) :
//            MaterialBase(reflectance), half_vertex_angle(half_vertex_angle) {}
//
//    std::tuple<Ray, double, Vector3> reflect(const Vector3 &point, const Vector3 &, const Vector3 &normal) const {
//        double theta = random_::rand() * half_vertex_angle;
//        double phi = random_::rand() * 2 * M_PI;
//
//        double x = std::cos(phi) * std::sin(theta);
//        double y = std::sin(phi) * std::sin(theta);
//        double z = std::cos(theta);
//
//        double pdf = ... * (1 - std::cos(half_vertex_angle));
//
//        auto const&[u, v] = vector3::tangent_space(normal);
//
//        Ray ray{point + normal * 0.001, x * u + y * v + z * normal, -1};
//        return {ray, pdf, reflectance ...};
//    };
//};

// 物理ベースではない
struct Fuzz : public MaterialBase {
    double half_vertex_angle;

    Fuzz(const Vector3 &reflectance, double half_vertex_angle) : MaterialBase(reflectance),
                                                                 half_vertex_angle(half_vertex_angle) {}

    std::tuple<Ray, Vector3> reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const {

        double theta = half_vertex_angle * random_::rand();
        double phi = random_::rand() * 2 * M_PI;

        double x = std::cos(phi) * std::sin(theta);
        double y = std::sin(phi) * std::sin(theta);

        auto const&[u, v] = vector3::tangent_space(normal);

        Ray next_ray{point + normal * 0.001,
                     in_direction.unit() - vector3::dot(normal.unit(), in_direction.unit()) * normal.unit() * 2 +
                     u * x + y * v, -1};
//        double cos = std::max(vector3::dot(normal.unit(), next_ray.direction.unit()), 0.);
        return {next_ray, reflectance};
    }
};

#endif //RENDERER_MATERIAL_H
