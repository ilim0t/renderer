//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <optional>
#include <vector>
#include "hit.h"
#include "ray.h"

struct Scene {
    std::vector<ShapeBase*> objects;

    Scene(const std::vector<ShapeBase*>& objects): objects(objects) {}

    std::optional<Hit> intersect(const Ray& ray) const {
        std::optional<Hit> hit, min_hit;
        double min_distance;
        for(const ShapeBase *object : objects) {
            std::optional<Hit> hit = object->intersect(ray);
            if(!hit) {
                continue;
            }
            if(!min_hit or hit->distance < min_hit->distance){
                min_hit = hit;
            }
        }
        return min_hit;
    }
};
#endif //RENDERER_SCENE_H
