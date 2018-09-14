//
// Created by ilim0t on 2018/09/14.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H
#include <vector>
#include <optional>
#include "hit.h"
#include "ray.h"
#include "object.h"

struct Scene {
    std::vector<Sphere> objects{
            {Vector(), 1}
    };

    std::optional<Hit> intersect(const Ray& ray, double dmax, double dmin) const {
        std::optional<Hit> minhit;
        for (const auto& object : objects) {
            const auto hit = object.intersect(ray, dmin, dmax);
            if (!hit) {
                continue;
            }
            minhit = hit;
            dmax = minhit->distance;
        }
        if (minhit) {
            const Sphere* o = minhit->object;
            minhit->point = ray.origin + ray.direction * minhit->distance;
            minhit->normal = (minhit->point - o->position) / o->radius;
        }
        return minhit;
    }
};

#endif //RENDERER_SCENE_H
