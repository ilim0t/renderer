//
// Created by ilim on 2018/10/19.
//

#include <iostream>
#include <vector>

#include "vector.h"
#include "image.h"
#include "shape.h"
#include "ray.h"
#include "scene.h"
#include "camera.h"

int main() {
    Image img(256, 256);
    std::vector<ShapeBase*> objects;
    auto s1 = Sphere{Vector3(-.75, -.75, 0), 1.5, Vector3(), Vector3()};
    objects.push_back(&s1);
    auto s2 = Sphere{Vector3(.75, .75, 0), 1.5, Vector3(), Vector3()};
    objects.push_back(&s2);
//    Sphere(Vector3(2, 1, 1), 1, Vector3(), Vector3());
//    objects.push_back(Sphere(Vector3(2, 1, 1), 1, Vector3(), Vector3()));

    Scene scene{objects};
    PinholeCamera camera(img, Vector3(), Vector3(0, 0, -4));

//    #pragma omp parallel for schedule(dynamic, 1)
    for(int x=0; x<img.width; ++x) {
        for(int y=0; y<img.height; ++y) {
            Vector3 L;
            Ray ray = camera.get_ray(x, y);
            auto hit = scene.intersect(ray);
            if(hit) {
                L = Vector3(hit->normal.abs());
            } else {
                L = Vector3(0.5, 0.7, 0.9);
            }
            img.set_pixel(x, y, L);
        }
    }
    img.output_ppm("./result.ppm");
    return 0;
}
