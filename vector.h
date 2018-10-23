//
// Created by ilim on 2018/10/19.
//

#ifndef RENDERER_VECTOR_H
#define RENDERER_VECTOR_H

#include <iostream>
#include <cmath>
#include <tuple>

struct Vector3;
namespace vector3 {
    double dot(const Vector3 &a, const Vector3 &b);
}

Vector3 operator/(const Vector3 &v, double k);

struct Vector3 {
    double x;
    double y;
    double z;

    Vector3(double k = 0) : x(k), y(k), z(k) {}

    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

    double length2() const {
        return vector3::dot(*this, *this);
    }

    double length() const {
        return std::sqrt(length2());
    }

    Vector3 unit() const {
        return *this / length();
    }

    Vector3 abs() const {
        return Vector3(std::abs(x), std::abs(y), std::abs(z));
    }

};

Vector3 operator+(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3 &v) {
    return Vector3(-v.x, -v.y, -v.z);
}

Vector3 operator-(const Vector3 &a, const Vector3 &b) {
    return a + (-b);
}

Vector3 operator*(const Vector3 &v, double k) {
    return Vector3(v.x * k, v.y * k, v.z * k);
}

Vector3 operator/(const Vector3 &v, double k) {
    return v * (1 / k);
}

Vector3 operator*(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

bool operator==(const Vector3 &a, const Vector3 &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
bool operator!=(const Vector3 &a, const Vector3 &b) {
    return !(a == b);
}

std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
    os << int(v.x) << " " << int(v.y) << " " << int(v.z) << "\n";
    return os;
}

namespace vector3 {
    double dot(const Vector3 &a, const Vector3 &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector3 cross(const Vector3 &a, const Vector3 &b) {
        return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
    }

    std::tuple<Vector3, Vector3> tangent_space(const Vector3 &normal) {
        Vector3 near = std::abs(normal.x) > 0.9 ? Vector3(normal.x, 1, 0) : Vector3(normal.x, 0, 0);
        Vector3 u = cross(normal, near).unit();
        Vector3 v = cross(u, normal).unit();
        return {u, v};
    }
}

#endif //RENDERER_VECTOR_H
