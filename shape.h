//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_SHAPE_H
#define RENDERER_SHAPE_H


#include <optional>
#include "vector.h"
#include "hit.h"
#include "ray.h"

struct ShapeBase {
    Vector3 reflectance;
    Vector3 illuminance;

    ShapeBase(const Vector3& reflectance, const Vector3& illuminance): reflectance(reflectance), illuminance(illuminance) {}
    virtual std::optional<Hit> intersect(const Ray& ray) const = 0;
    virtual Vector3 reflect(const Vector3& point, const Vector3& in_direction, const Vector3& normal) const = 0;
};

struct Sphere : public ShapeBase {
    Vector3 position;
    double radius;
//    Vector3 reflectance;
//    Vector3 illuminance;

    Sphere(Vector3 position, double radius, Vector3 reflectance, Vector3 illuminance) :
            ShapeBase(reflectance, illuminance), position(position), radius(radius){}

    std::optional<Hit> intersect(const Ray& ray) const {
            Vector3 position_origin = ray.origin - position;

            double a = ray.direction.length2();
            double b = 2 * vector3::dot(ray.direction, position_origin);
            double c = position_origin.length2() - radius*radius;

            double D = b*b - 4*a*c;
            if(D < 0) {
                    return {};
            }
            double short_point = (-b - std::sqrt(D)) / (2*a);
            if(short_point > 0) {
                    Vector3 point(ray.origin + short_point * ray.direction);
                    return Hit{point, (point - position).unit(), (point-ray.origin).length(), this};
            }
            double long_point = (-b + std::sqrt(D)) / (2*a);
            if(long_point > 0) {
                    Vector3 point(ray.origin + long_point * ray.direction);
                    return Hit{point, (point - position).unit(), (point-ray.origin).length(), this};
            }
            return {};
    }

    virtual Vector3 reflect(const Vector3& point, const Vector3& in_direction, const Vector3& normal) const {
        Vector3 v = in_direction.unit() - vector3::dot(normal.unit(), in_direction.unit()) * normal.unit() * 2;
        return v;
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
