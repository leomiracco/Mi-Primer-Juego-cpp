#pragma once
#include <vector>
#include <cstdint>

class BoxCollider;

struct CollisionPair {
    BoxCollider* a = nullptr;
    BoxCollider* b = nullptr;

    bool operator==(const CollisionPair& other) const {
      return (a == other.a && b == other.b) || (a == other.b && b == other.a);
    }
};

class CollisionSystem {
  public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;

    void Register(BoxCollider* collider);
    void Unregister(BoxCollider* collider);
    void Update();
    void Clear();

  private:
    std::vector<BoxCollider*> colliders;
    std::vector<CollisionPair> activePairs;
};