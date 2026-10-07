#pragma once
#include <cstddef>
#include <type_traits>

class GameObject;

using ComponentTypeID = std::size_t;

namespace Internal {
  inline ComponentTypeID GetUniqueComponentTypeID() noexcept {
    static ComponentTypeID lastID = 0;
    return lastID++;
  }
}

template <typename T>
inline ComponentTypeID GetComponentTypeID() noexcept {
  static_assert(std::is_base_of_v<class Component, T>, "T debe heredar de Component");
  static ComponentTypeID typeID = Internal::GetUniqueComponentTypeID();
  return typeID;
}

class Component {
  public:
    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Start() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}

    virtual void OnCollisionEnter(GameObject* other) {}
    virtual void OnCollisionStay(GameObject* other) {}
    virtual void OnCollisionExit(GameObject* other) {}

    GameObject* gameObject = nullptr;
    ComponentTypeID typeID = 0;
    bool hasStarted = false;
};