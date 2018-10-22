//
// Created by ilim on 2018/10/20.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <cmath>
#include "ray.h"
#include "vector.h"
#include "image.h"
#include "random.h"

struct CameraBase {
    Image image;
    Vector3 up;
    Vector3 direction;

    CameraBase(const Image &image, const Vector3 &direction, const Vector3 &up = Vector3()) :
            image(image), up(up), direction(direction) {}

    virtual Ray get_ray(int x, int y) const = 0;
};

struct PinholeCamera : public CameraBase {
    Vector3 target;
    Vector3 position;
    double distance;
    Vector3 rightward;
    Vector3 upward;

    PinholeCamera(const Image &image, const Vector3 &position, const Vector3 &target, double distance,
                  const Vector3 &up = Vector3(0, 1, 0)) :
            CameraBase(image, (target - position).unit(), up), target(target), position(position), distance(distance),
            rightward(vector3::cross(up, direction).unit()), upward(vector3::cross(direction, rightward).unit()) {}

    Ray get_ray(int x, int y) const {
        Ray ray;
        ray.origin = position + rightward * (1 - 2. * (x + random_::rand()) / image.width) +
                     upward * (2. * (y + random_::rand()) / image.height - 1);
        ray.direction = (position + direction * distance - ray.origin).unit();
        ray.depth = 0;
        return ray;
    }
};

struct NormalCamera : public CameraBase {
    Vector3 position;
    double fov;
    Vector3 rightward;
    Vector3 upward;

    NormalCamera(const Image &image, const Vector3 &position, const Vector3 &target, double fov,
                 const Vector3 &up = Vector3(0, 1, 0)) :
            CameraBase(image, (target - position).unit(), up), position(position), fov(fov),
            rightward(vector3::cross(up, direction).unit()), upward(vector3::cross(direction, rightward).unit()) {}

    Ray get_ray(int x, int y) const {
        Ray ray;
        ray.origin = position;
        Vector3 v = Vector3(2. * (x + random_::rand()) / image.width - 1, 1 - 2. * (y + random_::rand()) / image.height,
                            1 / std::tan(fov * 0.5)).unit();
        ray.direction = v.x * rightward + v.y * upward + v.z * direction.unit();
        ray.depth = 0;
        return ray;
    }
};

#endif //RENDERER_CAMERA_H
