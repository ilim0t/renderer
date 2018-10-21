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
    std::vector<ShapeBase*> shapes{
            new Sphere(Vector3(1, 0, 0), 1, Vector3(0.9, 0.3, 0.3), Vector3()),
            new Sphere(Vector3(-1, 0, 0), 1, Vector3(0.3, 0.9, 0.3), Vector3()),
            new Sphere(Vector3(0, 9, 0), 5, Vector3(0), Vector3(1))

    };
    Scene scene(shapes, Vector3(0.3), 1);
    PinholeCamera camera(img, Vector3(0), Vector3(0, 0, -4));

    #pragma omp parallel for schedule(dynamic, 1)
    for(int x=0; x<img.width; ++x) {
        for(int y=0; y<img.height; ++y) {
            Vector3 L(0);
            Ray ray = camera.get_ray(x, y);
            for(int spp=0; spp<scene.spp; ++spp) {
                L = L + scene.L(ray);
            }
            img.set_pixel(x, y, L/scene.spp);
        }
    }
    img.output_ppm("./result.ppm");
    return 0;
}
