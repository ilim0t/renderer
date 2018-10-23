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
    std::vector<ShapeBase *> shapes;
    Vector3 sky_color;
    int spp;
    int lim_depth;

    Scene(const std::vector<ShapeBase *> shapes, const Vector3 &sky_color, int spp, int(lim_depth)) :
            shapes(shapes), sky_color(sky_color), spp(spp), lim_depth(lim_depth) {}

    std::optional<Hit> intersect(const Ray &ray) const {
        std::optional<Hit> hit, min_hit;
        for (const auto &shape : shapes) {
            std::optional<Hit> hit = shape->intersect(ray);
            if (!hit) {
                continue;
            }
            if (!min_hit or hit->distance < min_hit->distance) {
                min_hit = hit;
            }
        }
        return min_hit;
    }

    Vector3 L(const Ray &ray, double roulette_p) {
        auto hit = intersect(ray);
        if (!hit || ray.depth >= lim_depth) {
            return Vector3();
        }
        Vector3 L_illuminance = hit->hit_shape_ptr->illuminance;

        auto[next_ray, f] = hit->hit_shape_ptr->reflect(hit->point, ray.direction, hit->normal);
        next_ray.depth = ray.depth + 1;

        Vector3 next_L = random_::rand() < roulette_p && f != Vector3() ? L(next_ray, roulette_p) : Vector3();
        return L_illuminance + next_L * f / roulette_p;
    }
};

#endif //RENDERER_SCENE_H
