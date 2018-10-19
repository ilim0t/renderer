//
// Created by ilim on 2018/10/19.
//

#include <iostream>
#include "vector.h"
#include "image.h"
#include "object.h"
#include "ray.h"

int main() {
    Image img(256, 256);
    Sphere sphere(Vector3(), 1., Vector3(), Vector3());

    #pragma omp parallel for schedule(dynamic, 1)
    for(int x=0; x<img.width; ++x) {
        for(int y=0; y<img.height; ++y) {
            Vector3 L;
            Ray ray;
            ray.origin = Vector3(2.*x/img.width-1, 2.*y/img.height-1, -4);
            ray.direction = Vector3(0, 0, 1);
            ray.depth = 0;
            auto hit = sphere.intersect(ray);
            if(hit) {
                L = Vector3(1);
            } else {
                L = Vector3(0.5, 0.7, 0.9);
            }
            img.set_pixel(x, y, L);
        }
    }
    img.output_ppm("./result.ppm");
    return 0;
}
