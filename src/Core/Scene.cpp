#include "Scene.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include <algorithm>

Scene::~Scene() {
  isDestroying = true;
  collisionSystem.Clear(); // 👈 El motor de físicas se limpia solo
  activeRenderers.clear();
  pendingObjects.clear();
  gameObjects.clear();
}

void Scene::IntegratePendingObjects() {
  if (pendingObjects.empty()) return;

  for (auto& obj : pendingObjects) {
    gameObjects.push_back(std::move(obj));
  }
  pendingObjects.clear();
}

void Scene::Update(float deltaTime) {
  IntegratePendingObjects();

  // FASE 1: START
  for (size_t i = 0; i < gameObjects.size(); ++i) {
    if (gameObjects[i]->isAlive && gameObjects[i]->isActive) {
      gameObjects[i]->Start();
    }
  }

  // FASE 2: UPDATE
  for (size_t i = 0; i < gameObjects.size(); ++i) {
    if (gameObjects[i]->isAlive && gameObjects[i]->isActive) {
      gameObjects[i]->Update(deltaTime);
    }
  }

  // 👉 FASE 3: FÍSICAS (Una sola línea limpia delegada al CollisionSystem)
  if (deltaTime > 0.0f) {
    collisionSystem.Update();
  }

  // FASE 4: Limpieza diferida
  for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
    if (!(*it)->isAlive) {
      it = gameObjects.erase(it);
    } else {
      ++it;
    }
  }
}

void Scene::Render() {
  if (mainCamera != nullptr) {
    mainCamera->Clear();
  } else {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  std::stable_sort(activeRenderers.begin(), activeRenderers.end(),
    [](const MeshRenderer* a, const MeshRenderer* b) {
      return a->sortingOrder < b->sortingOrder;
    }
  );

  for (auto* renderer : activeRenderers) {
    if (renderer->gameObject && renderer->gameObject->isAlive && renderer->gameObject->isActive) {
      renderer->Render();
    }
  }
}

GameObject* Scene::CreateGameObject(const std::string& name) {
  auto newObj = std::make_unique<GameObject>(name);
  newObj->scene = this;
  GameObject* rawPtr = newObj.get();
  pendingObjects.push_back(std::move(newObj));
  return rawPtr;
}

void Scene::RegisterRenderer(MeshRenderer* renderer) {
  if (renderer != nullptr && !isDestroying) {
    activeRenderers.push_back(renderer);
  }
}

void Scene::UnregisterRenderer(MeshRenderer* renderer) {
  if (isDestroying || renderer == nullptr) return;

  for (size_t i = 0; i < activeRenderers.size(); ++i) {
    if (activeRenderers[i] == renderer) {
      activeRenderers[i] = activeRenderers.back();
      activeRenderers.pop_back();
      return;
    }
  }
}

GameObject* Scene::FindGameObject(const std::string& name) const {
  for (const auto& obj : gameObjects) {
    if (obj->isAlive && obj->name == name) return obj.get();
  }
  return nullptr;
}

GameObject* Scene::FindGameObjectWithTag(const std::string& tag) const {
  for (const auto& obj : gameObjects) {
    if (obj->isAlive && obj->tag == tag) return obj.get();
  }
  return nullptr;
}

std::vector<GameObject*> Scene::FindGameObjectsWithTag(const std::string& tag) const {
  std::vector<GameObject*> results;
  for (const auto& obj : gameObjects) {
    if (obj->isAlive && obj->tag == tag) results.push_back(obj.get());
  }
  return results;
}