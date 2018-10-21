//
// Created by ilim on 2018/10/20.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include "ray.h"
#include "vector.h"
#include "image.h"
#include "random.h"

struct PinholeCamera {
    Image image;
    Vector3 target;
    Vector3 position;
    Vector3 direction;
    Vector3 up;
    Vector3 rightward;
    Vector3 upward;

    PinholeCamera(const Image& image, const Vector3& target, const Vector3& position,
            const Vector3& up=Vector3(0, 1, 0)) :
                image(image), target(target), position(position), direction((target - position).unit()), up(up),
                rightward(vector3::cross(up, direction).unit()), upward(vector3::cross(direction, rightward).unit()) {}

    Ray get_ray(int x, int y) {
        Ray ray;
        ray.origin = position + rightward * (1 - 2. * (x + random_::rand()) / image.width) +
                                upward * (2. * (y + random_::rand()) / image.height - 1);
        ray.direction = (position + direction - ray.origin).unit();
        ray.depth = 0;
        return ray;
    }
};

#endif //RENDERER_CAMERA_H
