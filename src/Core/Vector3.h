#pragma once
#include <cmath>
#include <algorithm> // 👈 OBLIGATORIO para std::clamp

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

    // Operadores de igualdad
    bool operator==(const Vector3& other) const {
      return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vector3& other) const {
      return !(*this == other);
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

    // ==============================================================
    // 🧮 MATEMÁTICA ESTÁNDAR DE LA INDUSTRIA (Unity / Unreal)
    // ==============================================================

    static float Distance(const Vector3& a, const Vector3& b) {
      return (a - b).Magnitude();
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
      float clampedT = std::clamp(t, 0.0f, 1.0f); // 👈 Ahora compila perfecto con <algorithm>
      return a + (b - a) * clampedT;
    }

    static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t) {
      return a + (b - a) * t;
    }

    static float Dot(const Vector3& a, const Vector3& b) {
      return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    // 👉 NUEVO: PRODUCTO CRUZ (Esencial para 3D, perpendiculares y vectores directores)
    static Vector3 Cross(const Vector3& a, const Vector3& b) {
      return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
      );
    }

    // Constantes estáticas útiles
    static Vector3 Zero()    { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One()     { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 Up()      { return Vector3(0.0f, -1.0f, 0.0f); }
    static Vector3 Down()    { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Left()    { return Vector3(-1.0f, 0.0f, 0.0f); }
    static Vector3 Right()   { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Back()    { return Vector3(0.0f, 0.0f, -1.0f); }
};