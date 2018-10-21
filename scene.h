//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <optional>
#include <vector>
#include "hit.h"
#include "ray.h"
#include "shape.h"

struct Scene {
    std::vector<ShapeBase*> shapes;
    Vector3 sky_color;
    int spp;

    Scene(const std::vector<ShapeBase*> shapes, const Vector3& sky_color, int spp):
    shapes(shapes), sky_color(sky_color), spp(spp) {}

    std::optional<Hit> intersect(const Ray& ray) const {
        std::optional<Hit> hit, min_hit;
        for(const auto& shape : shapes) {
            std::optional<Hit> hit = shape->intersect(ray);
            if(!hit) {
                continue;
            }
            if(!min_hit or hit->distance < min_hit->distance){
                min_hit = hit;
            }
        }
        return min_hit;
    }

    Vector3 L(const Ray& ray) {
        auto hit = intersect(ray);
        if(!hit || ray.depth > 100) {
            return sky_color;
        }
        Vector3 L_illuminance = hit->hit_shape_ptr->illuminance;

        Ray next_ray;
        next_ray.origin = hit->point + hit->normal*0.01;
        next_ray.direction = hit->hit_shape_ptr->reflect(hit->point, ray.direction, hit->normal);
        next_ray.depth = ray.depth + 1;
        Vector3 next_L = L(next_ray);

        return next_L * hit->hit_shape_ptr->reflectance + L_illuminance;
    }
};
#endif //RENDERER_SCENE_H
