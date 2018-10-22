//
// Created by ilim on 2018/10/19.
//

#include <iostream>
#include <vector>

#include "vector.h"
#include "image.h"
#include "shape.h"
#include "scene.h"
#include "camera.h"


int main() {
    Image img(512, 512);
    std::vector<ShapeBase *> shapes{
            new Sphere(Vector3(1, 0, 0), 1, Vector3(), new Diffuse(Vector3(0.95, 0.5, 0.5))),
            new Sphere(Vector3(-1, 0, 0), 1, Vector3(), new Diffuse(Vector3(0.5, 0.95, 0.5))),
            new Sphere(Vector3(0, -4001, 0), 4000, Vector3(), new Mirror(Vector3(0.95))),
            new Sphere(Vector3(0, 8000, 0), 4000, Vector3(1), new Diffuse(Vector3(0))),
            new Sphere(Vector3(0, -0.8, 0), 0.2, Vector3(0.4, 0.4, 1), new Diffuse(Vector3(0)))

    };
    Scene scene(shapes, Vector3(0), 80, 100);
    PinholeCamera camera(img, Vector3(0), Vector3(0, 0, -4));

    #pragma omp parallel for schedule(auto)
    for (int x = 0; x < img.width; ++x) {
        for (int y = 0; y < img.height; ++y) {
            Vector3 L(0);
            Ray ray = camera.get_ray(x, y);
            for (int spp = 0; spp < scene.spp; ++spp) {
                L = L + scene.L(ray, 0.95);
            }
            img.set_pixel(x, y, L / scene.spp);
        }
    }
    img.output_ppm("./result.ppm");
    return 0;
}
