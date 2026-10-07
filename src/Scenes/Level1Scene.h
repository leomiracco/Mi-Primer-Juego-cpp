#pragma once
#include "Core/Scene.h"
#include "Graphics/Mesh.h"            // 👈 Graphics
#include "Gameplay/Layers.h" // 👈 1. Incluimos las Capas de Física
#include "Gameplay/GameManager.h"      // 👈 Gameplay
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Gameplay/Health.h"           // 👈 Gameplay
#include "Gameplay/PlayerController.h" // 👈 Gameplay
#include "Components/BoxCollider.h"
#include "Gameplay/BulletPool.h"
#include "Components/Camera.h"

class Level1Scene : public Scene {
  public:
    void Init() override {
      // 0. CÁMARA PRINCIPAL (Centrada en el medio de la pantalla)
      auto* camObj = CreateGameObject("MainCamera");
      camObj->AddComponent<Transform>(); // Posición (0, 0, 0)
      //camObj->AddComponent<Transform>(Screen::GetWidthF() * 0.5f, Screen::GetHeightF() * 0.5f);
      camObj->AddComponent<Camera>();

      // 🎮 GESTOR DE REGLAS GLOBALES (Gobierna pausa, reinicios y estado)
      auto* gameManagerObj = CreateGameObject("GameManager");
      gameManagerObj->AddComponent<GameManager>();

      // Malla compartida (Quad)
      auto quadMesh = Mesh::CreateQuad();

      // 1. JUGADOR (Verde)
      auto* player = CreateGameObject("Player");
      player->tag = "Player";
      auto* playerTransform = player->AddComponent<Transform>(100.0f, 250.0f, 80.0f, 80.0f);
      player->AddComponent<MeshRenderer>(quadMesh, glm::vec4(50.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f, 1.0f));
      player->AddComponent<PlayerController>();
      player->AddComponent<BulletPool>(); // 👈 El jugador nace con su pool de munición
      
      // Colisionador del Jugador con Capas
      auto* playerCollider = player->AddComponent<BoxCollider>();
      playerCollider->layer = Layer::Player;
      playerCollider->collisionMask = Layer::Enemy | Layer::EnemyBullet; // Solo choca con amenazas

      // 🛡️ ESCUDO CELESTE (Hijo soldado al jugador)
      auto* shield = CreateGameObject("PlayerShield");
      auto* shieldTransform = shield->AddComponent<Transform>(1.0f, 0.25f, 0.25f, 0.5f);
      // 👉 sortingOrder = 1 (Garantiza que siempre se dibuje sobre el cuerpo de la nave que tiene sortingOrder = 0)
      auto* shieldRender = shield->AddComponent<MeshRenderer>(quadMesh, glm::vec4(0.2f, 0.8f, 1.0f, 1.0f), 1);
      shieldTransform->SetParent(playerTransform);

      // 2. ENEMIGO (Rojo - Con 3 puntos de vida)
      auto* enemy = CreateGameObject("Enemy");
      enemy->tag = "Enemy";
      enemy->AddComponent<Transform>(550.0f, 250.0f, 80.0f, 80.0f);
      enemy->AddComponent<MeshRenderer>(quadMesh, glm::vec4(0.85f, 0.2f, 0.2f, 1.0f));
      
      // 👉 LE ASIGNAMOS SALUD: Requiere 3 disparos para ser destruido
      enemy->AddComponent<Health>(3);
      
      // Colisionador del Enemigo con Capas
      auto* enemyCollider = enemy->AddComponent<BoxCollider>();
      enemyCollider->layer = Layer::Enemy;
      enemyCollider->collisionMask = Layer::Player | Layer::PlayerBullet; // Choca con jugador y sus balas
    }
};