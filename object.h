//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H


#include <optional>
#include "vector.h"
#include "hit.h"
#include "ray.h"

struct BaseObject {
    virtual std::optional<Hit> intersect(const Ray& ray) const = 0;
};

struct Sphere : public BaseObject {
    Vector3 position;
    double radius;
    Vector3 reflectance;
    Vector3 illuminance;

    Sphere(Vector3 position, double radius, Vector3 reflectance, Vector3 illuminance) :
            position(position), radius(radius), reflectance(reflectance), illuminance(illuminance){}

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
};


#endif //RENDERER_OBJECT_H
