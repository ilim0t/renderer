//
// Created by ilim0t on 2018/09/13.
//

#ifndef RENDERER_VECTOR_H
#define RENDERER_VECTOR_H
#include <fstream>
#include <cmath>

struct Vector {
    double x;
    double y;
    double z;

    Vector(double v = 0) : Vector(v, v, v) {};
    Vector(double x, double y, double z): x(x), y(y), z(z) {};

    double operator[](int i) const {
        return (&x)[i];
    }
};

Vector operator+(const Vector& a, const Vector& b) {
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector operator-(const Vector& a, const Vector& b) {
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector operator*(double k, const Vector& v) {
    return Vector(k * v.x, k * v.y, k * v.z);
}

Vector operator*(const Vector& a, const Vector& b) {
    return Vector(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector operator/(const Vector& a, const Vector& b) {
    return Vector(a.x / b.x, a.y / b.y, a.z / b.z);
}

Vector operator/(double k, const Vector& v) {
    return Vector(v.x / k, v.y / k, v.z /k);
}

Vector operator-(const Vector& v) {
    return Vector(- v.x, - v.y, - v.z);
}

double dot(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector cross(const Vector& a, const Vector& b) {
    return Vector(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.z
    );
}

Vector normalize(const Vector& v) {
    return v / std::sqrt(dot(v, v));
}


//void operator<<(std::ofstream& stream, const Vector& v) {
//    stream << "(" << v.x << ", " << v.y <<  ", " << v.z << ")" << std::endl;
//}

//std::ofstream& operator<<(std::ofstream& stream, const Vector& v) {
//    stream << "(" << v.x << ", " << v.y <<  ", " << v.z << ")";
//    return stream;
//}

#endif //RENDERER_VECTOR_H
