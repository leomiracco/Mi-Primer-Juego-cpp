#pragma once
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Vector3.h"

class Transform : public Component {
  public:
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 scale    = Vector3(1.0f, 1.0f, 1.0f);

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    Transform() = default;

    Transform(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)
      : position(x, y, 0.0f), scale(scaleX, scaleY, scaleZ) {}

    Transform(const Vector3& pos, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
      : position(pos), scale(scale) {}

    ~Transform() override {
      if (parent != nullptr) {
        parent->RemoveChild(this);
      }
      for (auto* child : children) {
        if (child != nullptr && child->gameObject != nullptr) {
          child->gameObject->Destroy();
          child->parent = nullptr;
        }
      }
    }

    void SetParent(Transform* newParent) {
      if (parent == newParent) return;

      if (parent != nullptr) {
        parent->RemoveChild(this);
      }

      parent = newParent;

      if (parent != nullptr) {
        parent->children.push_back(this);
      }

      SetDirty(); // Re-calcula matrices ante cambio de padre
    }

    // 👉 MARCA ESTE NODO Y A TODOS SUS HIJOS EN CASCADA COMO SUCIOS
    void SetDirty() const {
      isDirty = true;
      for (auto* child : children) {
        if (child != nullptr) {
          child->SetDirty();
        }
      }
    }

    glm::mat4 GetLocalMatrix() const {
      // Si hubo cambios locales, recalculamos la matriz local y ensuciamos la jerarquía
      if (position != lastPos || rotation != lastRot || scale != lastScale) {
        cachedLocalMatrix = glm::mat4(1.0f);
        cachedLocalMatrix = glm::translate(cachedLocalMatrix, glm::vec3(position.x, position.y, position.z));
        
        if (rotation.x != 0.0f) cachedLocalMatrix = glm::rotate(cachedLocalMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        if (rotation.y != 0.0f) cachedLocalMatrix = glm::rotate(cachedLocalMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        if (rotation.z != 0.0f) cachedLocalMatrix = glm::rotate(cachedLocalMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        cachedLocalMatrix = glm::scale(cachedLocalMatrix, glm::vec3(scale.x, scale.y, scale.z));

        lastPos = position;
        lastRot = rotation;
        lastScale = scale;
        SetDirty(); // Avisa a todos los hijos que el padre cambió
      }

      return cachedLocalMatrix;
    }

    // 👉 MATRIZ DE MUNDO CON CACHÉ TOTAL (Solo se multiplica 1 vez por frame)
    glm::mat4 GetWorldMatrix() const {
      GetLocalMatrix(); // Asegura que la local esté al día

      if (isDirty) {
        if (parent != nullptr) {
          cachedWorldMatrix = parent->GetWorldMatrix() * cachedLocalMatrix;
        } else {
          cachedWorldMatrix = cachedLocalMatrix;
        }
        isDirty = false; // Limpia la bandera: las siguientes llamadas en este frame costarán 0
      }

      return cachedWorldMatrix;
    }

    Vector3 GetWorldPosition() const {
      glm::mat4 world = GetWorldMatrix();
      return Vector3(world[3][0], world[3][1], world[3][2]);
    }

    Vector3 GetWorldScale() const {
      glm::mat4 world = GetWorldMatrix();
      float sx = glm::length(glm::vec3(world[0]));
      float sy = glm::length(glm::vec3(world[1]));
      float sz = glm::length(glm::vec3(world[2]));
      return Vector3(sx, sy, sz);
    }

    void Translate(const Vector3& delta) { 
      position += delta; 
      SetDirty();
    }
    void Translate(float deltaX, float deltaY, float deltaZ = 0.0f) {
      position.x += deltaX; position.y += deltaY; position.z += deltaZ;
      SetDirty();
    }

  private:
    void RemoveChild(Transform* child) {
      children.erase(std::remove(children.begin(), children.end(), child), children.end());
      SetDirty();
    }

    mutable Vector3 lastPos = Vector3(999999.0f, 999999.0f, 999999.0f);
    mutable Vector3 lastRot = Vector3(999999.0f, 999999.0f, 999999.0f);
    mutable Vector3 lastScale = Vector3(999999.0f, 999999.0f, 999999.0f);
    
    mutable glm::mat4 cachedLocalMatrix = glm::mat4(1.0f);
    mutable glm::mat4 cachedWorldMatrix = glm::mat4(1.0f); // 👈 Caché de mundo acumulada
    mutable bool isDirty = true;                           // 👈 Bandera de sincronización jerárquica
};