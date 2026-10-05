#pragma once
#include "Core/Scene.h"
#include "Core/Mesh.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h" // 👈 Usamos MeshRenderer universal
#include "Components/PlayerController.h"
#include "Components/BoxCollider.h"
#include "Components/Camera.h"

class Level1Scene : public Scene {
  public:
    void Init() override {
      // 0. CÁMARA PRINCIPAL
      auto* camObj = CreateGameObject("MainCamera");
      camObj->AddComponent<Transform>();
      mainCamera = camObj->AddComponent<Camera>(800, 600);

      // 🌟 RECURSO COMPARTIDO: Un solo Quad en la VRAM para toda la escena
      auto quadMesh = Mesh::CreateQuad();

      // 1. JUGADOR (Verde)
      auto* player = CreateGameObject("Player");
      player->AddComponent<Transform>(100.0f, 250.0f, 80, 80);
      player->AddComponent<MeshRenderer>(quadMesh, glm::vec4(50.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f, 1.0f));
      player->AddComponent<PlayerController>();
      player->AddComponent<BoxCollider>();

      // 2. ENEMIGO (Rojo)
      auto* enemy = CreateGameObject("Enemy");
      enemy->AddComponent<Transform>(550.0f, 250.0f, 80, 80);
      enemy->AddComponent<MeshRenderer>(quadMesh, glm::vec4(0.85f, 0.2f, 0.2f, 1.0f));
      enemy->AddComponent<BoxCollider>();
    }
};