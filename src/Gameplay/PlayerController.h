#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Input.h"
#include "Gameplay/BulletPool.h" // 👈 Usamos BulletPool
#include "Components/Transform.h"

class PlayerController : public Component {
  public:
    float speed = 250.0f;
    float fireCooldown = 0.0f;
    Transform* transform = nullptr;
    BulletPool* bulletPool = nullptr;

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    // 👉 EN START: Solo busca su BulletPool ya creado e inicializa las 30 balas
    void Start() override {
      bulletPool = gameObject->GetComponent<BulletPool>();
      if (bulletPool != nullptr) {
        bulletPool->InitPool(30); // Pre-calienta 30 balas en memoria
      }
    }

    void Update(float deltaTime) override {
      if (transform != nullptr) {
        // 1. Leemos la intención del jugador
        float moveX = Input::GetAxisHorizontal();
        float moveY = Input::GetAxisVertical();

        // 👉 2. NORMALIZACIÓN VECTORIAL:
        // Si presiona W y D a la vez, evita que corra un 41.4% más rápido
        Vector3 moveDir(moveX, moveY, 0.0f);
        if (moveDir.Magnitude() > 0.0f) {
          moveDir = moveDir.Normalized(); // Garantiza longitud exacta de 1.0f
        }

        // 3. Movemos al personaje a velocidad constante y uniforme en 360 grados
        transform->position += moveDir * speed * deltaTime;

        // 4. Temporizador de cadencia de disparo
        if (fireCooldown > 0.0f) {
          fireCooldown -= deltaTime;
        }

        // 5. Disparo reutilizando balas del BulletPool (Cero 'new' en el Heap)
        if (Input::GetButtonFire() && fireCooldown <= 0.0f) {
          float spawnX = transform->position.x + transform->scale.x;
          float spawnY = transform->position.y + (transform->scale.y * 0.5f) - 6.0f;

          if (bulletPool != nullptr) {
            bulletPool->Spawn(spawnX, spawnY);
          }
          fireCooldown = 0.25f;
        }

        // 🔮 Teletransporte al cursor mediante Camera::GetMain()
        if (Input::GetMouseButtonDown(MouseButton::Right)) {
          Camera* cam = Camera::GetMain();
          if (cam != nullptr) {
            Vector3 mouseWorld = cam->ScreenToWorldPoint(Input::GetMousePosition());
            transform->position.x = mouseWorld.x;
            transform->position.y = mouseWorld.y;
          }
        }
      }
    }
};