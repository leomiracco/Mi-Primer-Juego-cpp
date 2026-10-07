#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Core/Scene.h"
#include "Gameplay/Layers.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/BoxCollider.h"
#include "Gameplay/Bullet.h"

class Prefabs {
  public:
    // Ensambla una bala en la escena y la deja apagada lista para el Pool
    static GameObject* CreateBullet(Scene* scene) {
      if (scene == nullptr) return nullptr;

      static std::shared_ptr<Mesh> bulletMesh = Mesh::CreateQuad();
      static std::shared_ptr<Material> bulletMaterial = std::make_shared<Material>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

      auto* bullet = scene->CreateGameObject("Bullet");
      bullet->tag = "Bullet";
      bullet->AddComponent<Transform>(0.0f, 0.0f, 24.0f, 12.0f);
      bullet->AddComponent<MeshRenderer>(bulletMesh, bulletMaterial, 5);
      bullet->AddComponent<Bullet>();

      auto* bulletCollider = bullet->AddComponent<BoxCollider>();
      bulletCollider->layer = Layer::PlayerBullet;
      bulletCollider->collisionMask = Layer::Enemy;

      // Nace apagada esperando en el Pool
      bullet->SetActive(false);

      return bullet;
    }
};