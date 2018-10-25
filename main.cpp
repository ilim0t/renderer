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
#include <limits.h>

#include "vector.h"
#include "image.h"
#include "shape.h"
#include "scene.h"
#include "camera.h"

struct Progress {
    int num = 0;
    int total;
    std::chrono::system_clock::time_point start;
    double delta_t = 0;
    double previous_elapsed_time = 0;
    int count = 0;
    int unit;

    Progress(int total) : total(total), start(std::chrono::system_clock::now()), previous_elapsed_time(elapsed()),
                          unit(std::max(total / 400, 1)) {}

    void show() {
        double elapsed_time = elapsed();
        double percent = (double) num / total * 100;

        double recently_delta_t = num ? (elapsed_time - previous_elapsed_time) / (num - unit * count) : 0;
        if (num >= unit * (count + 1)) {
            count = num / unit;
            previous_elapsed_time = elapsed_time;
        }
        delta_t += 0.005 / std::max(elapsed_time, 1.) * (recently_delta_t - delta_t);
        double remain = delta_t * (total - num);
        std::cout << "\r" << std::fixed << std::setprecision(2) <<
                  percent << "%\tremain: " << (int) remain << "[s]\ttotal: " << elapsed_time << "[s]" << std::flush;
    }

    double elapsed() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() /
               1000.;
    }

    void end() {
        std::cout << "\nComplete: " << elapsed() << "[s]" << std::endl;
    }
};

int main() {
    Image img(256, 256);

     // コーネルボックス
//    std::vector<std::shared_ptr<ShapeBase>> shapes{
//            std::make_shared<Sphere>(Vector3(0, 10003.5, 0), 10000, Vector3(), Diffuse(Vector3(0.75))), // 上壁
//            std::make_shared<Sphere>(Vector3(10004, 0, 0), 10000, Vector3(), Diffuse(Vector3(0.25, 0.25, 0.75))), //右壁
//            std::make_shared<Sphere>(Vector3(0, -10003.5, 0), 10000, Vector3(), Diffuse(Vector3(0.75))), // 下壁
//            std::make_shared<Sphere>(Vector3(-10004, 0, 0), 10000, Vector3(), Diffuse(Vector3(0.75, 0.25, 0.25))), // 左壁
//            std::make_shared<Sphere>(Vector3(0, 0, 10004), 10000, Vector3(0), Diffuse(Vector3(0.75))), // 奥壁
//            std::make_shared<Sphere>(Vector3(0, 23.48, 2), 20, Vector3(12), Diffuse(Vector3())), // ライト
//
//            std::make_shared<Sphere>(Vector3(-2, -2.3, 3), 1.2, Vector3(), Diffuse(Vector3(0.999))),
//            std::make_shared<Sphere>(Vector3(2, -2.3, 2), 1.2, Vector3(), Diffuse(Vector3(0.25, 0.75, 0.25))),
//            std::make_shared<ParallelLight>(Vector3(0, 0, -1), M_PI * 15/180, Vector3(1), Diffuse(Vector3(0))),
//    };
//    std::shared_ptr<CameraBase> camera = std::make_shared<NormalCamera>(
//            img, Vector3(0, 0, -6), Vector3(0), M_PI * 3 / 8);

    // 球
    std::vector<std::shared_ptr<ShapeBase>> shapes{
        // 床
        std::make_shared<Sphere>(Vector3(0, -10004, 0), 10000, Vector3(), Diffuse(Vector3(0.75))),
        // ピラミッド
        std::make_shared<Sphere>(Vector3(0.8, -3.2, 2+0.8*std::sqrt(3)), 0.8, Vector3(), Glass(Vector3(0.9), 1.51, 1)),
        std::make_shared<Sphere>(Vector3(-0.8, -3.2, 2+0.8*std::sqrt(3)), 0.8, Vector3(), Glass(Vector3(0.9), 1.51, 1)),
        std::make_shared<Sphere>(Vector3(0, -3.2, 2), 0.8, Vector3(), Glass(Vector3(0.9), 1.51, 1)),
        std::make_shared<Sphere>(Vector3(0, -3.2 + 2. / 3. * std::sqrt(6) * 0.8, 2+2*std::sqrt(3) * 0.8 / 3), 0.8,
                Vector3(), Fuzz(Vector3(0.9), M_PI*10/180)),
        // 奥 球
        std::make_shared<Sphere>(Vector3(-2, -2.5, 5), 1.5, Vector3(), Diffuse(Vector3(0.25, 0.75, 0.25))),
        std::make_shared<Sphere>(Vector3(2, -2.5, 5), 1.5, Vector3(), Diffuse(Vector3(0.75, 0.25, 0.25))),
        // 手前 球
        std::make_shared<Sphere>(Vector3(2, -3.2, 0), 0.8, Vector3(), Diffuse(Vector3(0.75))),
        std::make_shared<Sphere>(Vector3(-2, -3.2, 0), 0.8, Vector3(), Diffuse(Vector3(0.75))),
        std::make_shared<Sphere>(Vector3(0, -3.4, -1), 0.6, Vector3(), Diffuse(Vector3(0.25, 0.25, 0.75))),
        // 上空 鏡
        std::make_shared<Sphere>(Vector3(0, -1.25, 8.5), 2, Vector3(), Mirror(Vector3(0.9))),
        // ライト
        std::make_shared<ParallelLight>(Vector3(1, 2, 2), M_PI * 10/180, Vector3(2), Diffuse(Vector3(0))),
    };
    std::shared_ptr<CameraBase> camera = std::make_shared<NormalCamera>(
            img, Vector3(0, 1, -6), Vector3(0, -1, -1), M_PI * 1 / 3);

    Scene scene(shapes, Vector3(0.15, 0.2, 0.35), 256, INT_MAX);
    Progress progress(img.size);

#pragma omp parallel for schedule(dynamic, 1)
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
