//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_IMAGE_H
#define RENDERER_IMAGE_H

#include <fstream>
#include <cmath>
#include <algorithm>
#include "vector.h"

struct Image;
namespace image {
    void output_ppm(std::string file_name, const Image &image);
}

struct Image {
    int width;
    int height;
    int size;
    Vector3 *data;

    Image(int width, int height) : width(width), height(height), size(width * height) {
        data = new Vector3[size];
    }

    ~Image() {
//        delete data;
    }

    void set_pixel(int x, int y, const Vector3 v) {
        data[coord2index(x, y)] = v;
    }

    Vector3 get_pixel(int x, int y) const {
        return data[coord2index(x, y)];
    }

    void output_ppm(std::string file_name) const {
        image::output_ppm(file_name, *this);
    }

private:
    int coord2index(int x, int y) const {
        return x + y * width;
    }
};

namespace image {
    double tonemap(double k) {
        return std::pow(std::clamp(k, 0., 1.), 1. / 2.2) * 255;
    }

    Vector3 tonemap(const Vector3 &v) {
        return Vector3(tonemap(v.x), tonemap(v.y), tonemap(v.z));
    }

    void output_ppm(std::string file_name, const Image &image) {
        std::ofstream ofs(file_name);
        ofs << "P3\n" << image.width << " " << image.height << "\n255\n";
        for (int i = 0; i < image.size; ++i) {
            ofs << tonemap(image.data[i]);
        }
    }
}

#endif //RENDERER_IMAGE_H
