//
// Created by ilim on 2018/09/13.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <cmath>

#include "vector.h"
#include "ray.h"
#include "hit.h"
#include "scene.h"
#include "image.h"


int main() {
    Scene scene;
    const int width = std::pow(2, 8);
    const int height = std::pow(2, 8);
    Image image(width, height);

    for (int i=0; i < width*height; ++i){
        const int x = i % width;
        const int y = i / width;

        Ray ray(Vector(2.*((double)x/width)-1, 2.*((double)y/width)-1, 5),  Vector(0, 0, -1));
        const auto hit = scene.intersect(ray, 0, 1e+10);
        if (hit) {
            image.setPixel(x, y, hit->normal);
        } else {
            image.setPixel(x, y, Vector(0, 0, 0));
        }
    }
    image.ppmOutput();
    return 0;
}