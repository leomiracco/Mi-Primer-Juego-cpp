#include "CollisionSystem.h"
#include "Components/BoxCollider.h"
#include "ECS/GameObject.h"
#include <algorithm>

void CollisionSystem::Register(BoxCollider* collider) {
  if (collider == nullptr) return;
  if (std::find(colliders.begin(), colliders.end(), collider) == colliders.end()) {
    colliders.push_back(collider);
  }
}

void CollisionSystem::Unregister(BoxCollider* collider) {
  if (collider == nullptr) return;

  // 1. Quitar de colliders activos con Swap-and-Pop O(1)
  for (size_t i = 0; i < colliders.size(); ++i) {
    if (colliders[i] == collider) {
      colliders[i] = colliders.back();
      colliders.pop_back();
      break;
    }
  }

  // 2. Si un collider muere o se apaga, se borran sus pares de inmediato
  for (auto it = activePairs.begin(); it != activePairs.end(); ) {
    if (it->a == collider || it->b == collider) {
      it = activePairs.erase(it);
    } else {
      ++it;
    }
  }
}

void CollisionSystem::Update() {
  std::vector<CollisionPair> currentPairs;

  // 1. Detección entre los colliders activos (todos están garantizados vivos y activos)
  for (size_t i = 0; i < colliders.size(); ++i) {
    for (size_t j = i + 1; j < colliders.size(); ++j) {
      BoxCollider* colA = colliders[i];
      BoxCollider* colB = colliders[j];

      if (!colA->CanCollideWith(*colB)) continue;

      if (colA->CheckCollision(*colB)) {
        currentPairs.push_back({ colA, colB });
      }
    }
  }

  // 2. Evaluar ENTER y STAY
  for (const auto& pair : currentPairs) {
    auto it = std::find(activePairs.begin(), activePairs.end(), pair);

    if (it == activePairs.end()) {
      pair.a->gameObject->OnCollisionEnter(pair.b->gameObject);
      pair.b->gameObject->OnCollisionEnter(pair.a->gameObject);
    } else {
      pair.a->gameObject->OnCollisionStay(pair.b->gameObject);
      pair.b->gameObject->OnCollisionStay(pair.a->gameObject);
    }
  }

  // 3. Evaluar EXIT (estaban en activePairs pero se separaron)
  for (const auto& oldPair : activePairs) {
    auto it = std::find(currentPairs.begin(), currentPairs.end(), oldPair);

    if (it == currentPairs.end()) {
      oldPair.a->gameObject->OnCollisionExit(oldPair.b->gameObject);
      oldPair.b->gameObject->OnCollisionExit(oldPair.a->gameObject);
    }
  }

  activePairs = std::move(currentPairs);
}

void CollisionSystem::Clear() {
  colliders.clear();
  activePairs.clear();
}