//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_SHAPE_H
#define RENDERER_SHAPE_H


#include <optional>
#include <tuple>
#include <memory>
#include "vector.h"
#include "hit.h"
#include "ray.h"
#include "material.h"

struct ShapeBase {
//    Vector3 reflectance;
    Vector3 illuminance;
    std::shared_ptr<MaterialBase> material_ptr;

    ShapeBase(const Vector3 &illuminance, std::shared_ptr<MaterialBase> material_ptr) :
            illuminance(illuminance), material_ptr(material_ptr) {}

    virtual std::optional<Hit> intersect(const Ray &ray) const = 0;

    virtual std::tuple<Ray, Vector3>
    reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const = 0;
};

struct Sphere : public ShapeBase {
    Vector3 position;
    double radius;

    template <class T>
    Sphere(const Vector3 &position, double radius, const Vector3 &illuminance, T material_ptr) :
            ShapeBase(illuminance, std::make_shared<T>(material_ptr)), position(position), radius(radius) {}

    std::optional<Hit> intersect(const Ray &ray) const {
        Vector3 position_origin = ray.origin - position;

        double a = ray.direction.length2();
        double b = 2 * vector3::dot(ray.direction, position_origin);
        double c = position_origin.length2() - radius * radius;

        double D = b * b - 4 * a * c;
        if (D < 0) {
            return {};
        }
        double short_point = (-b - std::sqrt(D)) / (2 * a);
        if (short_point > 0) {
            Vector3 point(ray.origin + short_point * ray.direction);
            return Hit{point, (point - position).unit(), (point - ray.origin).length(), this};
        }
        double long_point = (-b + std::sqrt(D)) / (2 * a);
        if (long_point > 0) {
            Vector3 point(ray.origin + long_point * ray.direction);
            return Hit{point, (point - position).unit(), (point - ray.origin).length(), this};
        }
        return {};
    }

    std::tuple<Ray, Vector3> reflect(const Vector3 &point, const Vector3 &in_direction, const Vector3 &normal) const {
        return material_ptr->reflect(point, in_direction, normal);
    };

};

//struct Triangle : public ShapeBase {
//    Vector3 a;
//    Vector3 b;
//    Vector3 c;
//
//    std::optional<Hit> intersect(const Ray& ray) {
//        vector3::dot(vector3::cross(a - ray.origin, b - ray.origin), ray.direction);
//        return
//    }
//};
//
//struct Plane : public ShapeBase {
//    Vector3
//};

#endif //RENDERER_SHAPE_H
