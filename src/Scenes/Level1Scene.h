#pragma once
#include "Core/Scene.h"
#include "Components/Transform.h"
#include "Components/SquareRenderer.h"
#include "Components/PlayerController.h"
#include "Components/BoxCollider.h"
#include "Components/Camera.h"

class Level1Scene : public Scene {
  public:
    void Init() override {
      // 0. CÁMARA (Como la Main Camera de Unity)
      auto* camObj = CreateGameObject("MainCamera");
      camObj->AddComponent<Transform>(); // Posición (0,0,0) por defecto
      mainCamera = camObj->AddComponent<Camera>(800, 600); // 👈 Ahora compila perfecto

      // 1. JUGADOR (Verde)
      auto* player = CreateGameObject("Player");
      auto* playerTransform = player->AddComponent<Transform>(100.0f, 250.0f, 80, 80);
      player->AddComponent<SquareRenderer>();
      player->AddComponent<PlayerController>();
      player->AddComponent<BoxCollider>();

      // 2. ENEMIGO (Rojo)
      auto* enemy = CreateGameObject("Enemy");
      auto* enemyTransform = enemy->AddComponent<Transform>(550.0f, 250.0f, 80, 80);
      auto* enemyRender = enemy->AddComponent<SquareRenderer>(0.85f, 0.2f, 0.2f);
      enemy->AddComponent<BoxCollider>();
    }
};