//
// Created by ilim on 2018/10/21.
//

#ifndef RENDERER_RANDOM_H
#define RENDERER_RANDOM_H

#include <random>

//std::random_device rnd;
std::mt19937 mt;
std::uniform_real_distribution<double> dist(0, 1);

namespace random_ {
    double rand() {
        return dist(mt);
    }
}

#endif //RENDERER_RANDOM_H
