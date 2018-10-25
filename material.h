//
// Created by ilim on 2018/10/21.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include <cassert>
#include "vector.h"
#include "random.h"
#include "ray.h"

double MIN_DIFF = 1e-5;

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

        Ray next_ray{point + normal * MIN_DIFF, x * u + y * v + z * normal, -1};
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
        return {next_ray, reflectance};
    }
};

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

        Ray next_ray{point + normal * MIN_DIFF,
                     in_direction.unit() - vector3::dot(normal.unit(), in_direction.unit()) * normal.unit() * 2 +
                     u * x + y * v, -1};
        return {next_ray, reflectance};
    }
};

struct Glass : public MaterialBase {
    double inside_n;
    double outside_n;

    Glass(const Vector3 &reflectance, double inside_n, double outside_n) :
            MaterialBase(reflectance), inside_n(inside_n), outside_n(outside_n) {}

    std::tuple<Ray, Vector3> reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const {
        double from_cos = vector3::dot(normal.unit(), in_direction.unit());
        double n1 = from_cos > 0 ? inside_n : outside_n;
        double n2 = from_cos > 0 ? outside_n : inside_n;
        Vector3 direction_normal = from_cos > 0 ? normal : -normal;
        from_cos = std::abs(from_cos);

        double ref_p = fresnel(n1, n2, from_cos);

        if (random_::rand() < ref_p) {
            Ray next_ray{point + normal * MIN_DIFF,
                         in_direction.unit() - vector3::dot(direction_normal.unit(), in_direction.unit()) * direction_normal.unit() * 2,
                         -1};
            return {next_ray, reflectance};
        }
        Vector3 vertical = direction_normal.unit() * from_cos; // 垂直
        Vector3 horizontal = (in_direction.unit() - vertical); // 水平

//        double from_sin = std::sqrt(1 - from_cos * from_cos);

        Vector3 to_horizontal = horizontal * n1 / n2;
        double to_sin = to_horizontal.length();
        if (to_sin >= 1) {
            Ray next_ray{point - direction_normal * MIN_DIFF,
                         in_direction.unit() - vector3::dot(direction_normal.unit(), in_direction.unit()) * direction_normal.unit() * 2,
                         -1};
            return {next_ray, reflectance};
        }
        double to_cos = std::sqrt(1 - to_sin * to_sin);
        Vector3 to_vertical = direction_normal.unit() * to_cos;

        Ray next_ray{point + direction_normal * MIN_DIFF,
                     (to_vertical + to_horizontal).unit(), -1};
//        double l = (to_vertical + to_horizontal).length();
//        double a = vector3::dot(to_vertical, to_horizontal);
//        double b = vector3::dot(vertical, horizontal);
        return {next_ray, std::pow(n1 / n2, 2) * reflectance};

    };
private:
    double fresnel(double n1, double n2, double cos) const {
        double f0 = std::pow((n1 - n2) / (n1 + n2), 2);
        return f0 + (1 - f0) * std::pow(1 - cos, 5);
    }

};

#endif //RENDERER_MATERIAL_H
