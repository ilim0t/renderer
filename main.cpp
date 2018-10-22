//
// Created by ilim on 2018/10/19.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

#include "vector.h"
#include "image.h"
#include "shape.h"
#include "scene.h"
#include "camera.h"

struct Progress {
    int num;
    int total;

    Progress(int total) : num(0), total(total) {}

    void show() {
        double percent = double(num) / total * 100;
        std::cout << "\r" << percent << "%" << "[" << num << "/" << total << "]" << std::flush;
    }
};

int main() {
    Image img(256, 256);
    std::vector<ShapeBase *> shapes{
        new Sphere(Vector3(0, 10004, 0), 10000, Vector3(), new Diffuse(Vector3(0.75))), // 上壁
        new Sphere(Vector3(10004, 0, 0), 10000, Vector3(), new Diffuse(Vector3(0.25, 0.25, 0.75))), //右壁
        new Sphere(Vector3(0, -10004, 0), 10000, Vector3(), new Mirror(0.95)), // 下壁
        new Sphere(Vector3(-10004, 0, 0), 10000, Vector3(), new Diffuse(Vector3(0.75, 0.25, 0.25))), // 左壁
        new Sphere(Vector3(0, 0, 10004), 10000, Vector3(0), new Diffuse(Vector3(0.75))), // 奥壁
        new Sphere(Vector3(0, 0, -10004), 10000, Vector3(), new Diffuse(Vector3(0.75))), // 手前壁
        new Sphere(Vector3(0, 23.99, 2), 20, Vector3(12), new Diffuse(Vector3())), // ライト

        new Sphere(Vector3(-2, -3, 3), 1.2, Vector3(), new Diffuse(Vector3(0.999))),
        new Sphere(Vector3(2, -3, 2), 1.2, Vector3(), new Diffuse(Vector3(0.25, 0.75, 0.25))),
        new Sphere(Vector3(0, 0, 4), 1.5, Vector3(), new Mirror(1)),
    };
    Scene scene(shapes, Vector3(0.5), 256, 20);
    CameraBase *camera = new PinholeCamera(img, Vector3(0, 0, -4), Vector3(), 1);
    Progress progress(img.width);

    auto start = std::chrono::system_clock::now();

#pragma omp parallel for schedule(dynamic, 1)
    for (int x = 0; x < img.width; ++x) {
        for (int y = 0; y < img.height; ++y) {
            if (omp_get_thread_num() == 0) {
                progress.show();
            }
            Vector3 L(0);
            Ray ray = camera->get_ray(x, y);
            for (int spp = 0; spp < scene.spp; ++spp) {
                L = L + scene.L(ray, 0.85);
            }
            img.set_pixel(x, y, L / scene.spp);
        }
        ++progress.num;

    }

    img.output_ppm("./result.ppm");
    auto end = std::chrono::system_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nComplete: " << elapsed << "[s]" << std::endl;
    return 0;
}
