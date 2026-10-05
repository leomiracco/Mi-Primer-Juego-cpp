#pragma once
#include <cmath>

struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3() = default;
    Vector3(float x, float y, float z = 0.0f) : x(x), y(y), z(z) {}

    // Operadores aritméticos
    Vector3 operator+(const Vector3& other) const {
      return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator-(const Vector3& other) const {
      return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 operator*(float scalar) const {
      return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 operator/(float scalar) const {
      if (scalar != 0.0f) {
        return Vector3(x / scalar, y / scalar, z / scalar);
      }
      return *this;
    }

    Vector3& operator+=(const Vector3& other) {
      x += other.x; y += other.y; z += other.z; return *this;
    }
    Vector3& operator-=(const Vector3& other) {
      x -= other.x; y -= other.y; z -= other.z; return *this;
    }
    Vector3& operator*=(float scalar) {
      x *= scalar; y *= scalar; z *= scalar; return *this;
    }

    // Longitud del vector (magnitud)
    float Magnitude() const {
      return std::sqrt(x * x + y * y + z * z);
    }

    // Vector unitario (longitud = 1.0f)
    Vector3 Normalized() const {
      float mag = Magnitude();
      if (mag > 0.0001f) {
        return *this / mag;
      }
      return Vector3(0.0f, 0.0f, 0.0f);
    }

    // Constantes estáticas estilo Unity (Vector3.zero, Vector3.up, etc.)
    static Vector3 Zero()    { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One()     { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 Up()      { return Vector3(0.0f, -1.0f, 0.0f); } // En 2D SDL, -Y es hacia arriba
    static Vector3 Down()    { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Left()    { return Vector3(-1.0f, 0.0f, 0.0f); }
    static Vector3 Right()   { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); } // Eje Z para el futuro 3D
};