//
// Created by ilim on 2018/10/19.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <chrono>
#include <omp.h>
#include <iomanip>

#include "vector.h"
#include "image.h"
#include "shape.h"
#include "scene.h"
#include "camera.h"

struct Progress {
    int num;
    int total;
    std::chrono::system_clock::time_point start;
    double delta_t = 0;

    Progress(int total) : num(0), total(total) , start(std::chrono::system_clock::now()){}

    void show() {
        double percent = (double)num / total * 100;
        delta_t += 1. / std::max(elapsed(), 1.) * (elapsed() / std::max(num, 1) - delta_t);
        double remain = delta_t * (total - num);
        std::cout << "\r" << std::fixed << std::setprecision(2) <<
        percent << "%\tremain: " << (int)remain << "[s]" << std::flush;
    }

    double elapsed() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() / 1000.;
    }

    void end() {
        std::cout << "\nComplete: " << elapsed() << "[s]" << std::endl;
    }
};

int main() {
    Image img(256, 256);

    // コーネルボックス
    std::vector<std::shared_ptr<ShapeBase>> shapes{
        std::make_shared<Sphere>(Vector3(0, 10003.5, 0), 10000, Vector3(), Diffuse(Vector3(0.75))), // 上壁
        std::make_shared<Sphere>(Vector3(10004, 0, 0), 10000, Vector3(), Diffuse(Vector3(0.25, 0.25, 0.75))) , //右壁
        std::make_shared<Sphere>(Vector3(0, -10003.5, 0), 10000, Vector3(), Diffuse(Vector3(0.75))), // 下壁
        std::make_shared<Sphere>(Vector3(-10004, 0, 0), 10000, Vector3(), Diffuse(Vector3(0.75, 0.25, 0.25))), // 左壁
        std::make_shared<Sphere>(Vector3(0, 0, 10004), 10000, Vector3(0), Diffuse(Vector3(0.75))), // 奥壁
        std::make_shared<Sphere>(Vector3(0, 23.48, 2), 20, Vector3(12), Diffuse(Vector3())), // ライト

        std::make_shared<Sphere>(Vector3(-2, -2.3, 3), 1.2, Vector3(), Diffuse(Vector3(0.999))),
        std::make_shared<Sphere>(Vector3(2, -2.3, 2), 1.2, Vector3(), Diffuse(Vector3(0.25, 0.75, 0.25))),
};

    // 球
//    std::vector<std::shared_ptr<ShapeBase>> shapes{
//        std::make_shared<Sphere>(Vector3(0, -10004, 0), 10000, Vector3(), Diffuse(Vector3(0.75))),
//        std::make_shared<Sphere>(Vector3(-2, -2.8, 3), 1.2, Vector3(), Diffuse(Vector3(0.999))),
//        std::make_shared<Sphere>(Vector3(2, -2.8, 2), 1.2, Vector3(), Diffuse(Vector3(0.25, 0.75, 0.25))),
//        std::make_shared<Sphere>(Vector3(0, -3, 5), 1, Vector3(), Fuzz(Vector3(0.75), 0.2 * M_PI)),
//    };

    Scene scene(shapes, Vector3(0.8), 256, 100);
    std::shared_ptr<CameraBase> camera = std::make_shared<NormalCamera>(img, Vector3(0, 0, -6), Vector3(0), M_PI / 3);
//    std::shared_ptr<CameraBase> camera = std::make_shared<PinholeCamera>(img, Vector3(0, 0, -4), Vector3(), 1);
    Progress progress(img.size);

#pragma omp parallel for schedule(auto)
    for (int x = 0; x < img.width; ++x) {
        for (int y = 0; y < img.height; ++y) {
#ifdef NDEBUG
            if (omp_get_thread_num() == 0) {
                progress.show();
            }
#else
            progress.show();

#endif
            Vector3 L(0);
            for (int spp = 0; spp < scene.spp; ++spp) {
                Ray ray = camera->get_ray(x, y);
                L = L + scene.L(ray, 0.96);
            }
            ++progress.num;
            img.set_pixel(x, y, L / scene.spp);
        }
    }

    img.output_ppm("./result.ppm");
    progress.end();
    return 0;
}
