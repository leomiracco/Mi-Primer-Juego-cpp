#pragma once
#include "Core/Scene.h"
#include "Components/Transform.h"
#include "Components/SquareRenderer.h"
#include "Components/PlayerController.h"
#include "Components/BoxCollider.h"

class Level1Scene : public Scene {
  public:
    void Init() override {
      // 1. JUGADOR (Verde)
      auto* player = CreateGameObject("Player");
      auto* playerTransform = player->AddComponent<Transform>();
      playerTransform->position.x = 100.0f; playerTransform->position.y = 250.0f; playerTransform->width = 80; playerTransform->height = 80;
      player->AddComponent<SquareRenderer>();
      player->AddComponent<PlayerController>();
      player->AddComponent<BoxCollider>();

      // 2. ENEMIGO (Rojo)
      auto* enemy = CreateGameObject("Enemy");
      auto* enemyTransform = enemy->AddComponent<Transform>();
      enemyTransform->position.x = 550.0f; enemyTransform->position.y = 250.0f; enemyTransform->width = 80; enemyTransform->height = 80;
      auto* enemyRender = enemy->AddComponent<SquareRenderer>();
      enemyRender->r = 220; enemyRender->g = 50; enemyRender->b = 50;
      enemy->AddComponent<BoxCollider>();
    }
};