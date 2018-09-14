#include <iostream>
#include <fstream>
#include <vector>
#include <optional>

#include "vector.h"
#include "ray.h"
#include "hit.h"
#include "scene.h"
#include "image.h"


int main() {
    Scene scene;
    const int width = 256;
    const int height = 256;
    Image image(width, height);

    for (int i=0; i < width*height; ++i){
        const int x = i % width;
        const int y = i / width;

        Ray ray(Vector(2.*((double)x/width)-1, 2.*((double)y/width)-1, 5),  Vector(0, 0, -1));
        const auto hit = scene.intersect(ray, 0, 1e+10);
        if (hit) {
            image.setPixel(x, y, Vector(1, 0.6, 1));
        } else {
            image.setPixel(x, y, Vector(0, 0, 0));
        }
    }
    image.ppmOutput();
    return 0;
}