#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Core/Scene.h"
#include "Core/Mesh.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h" // 👈 Usamos MeshRenderer
#include "Components/BoxCollider.h"
#include "Components/Bullet.h"

class Prefabs {
  public:
    static GameObject* SpawnBullet(Scene* scene, float x, float y) {
      if (scene == nullptr) return nullptr;

      // 1. Malla compartida: se crea en la GPU solo la primera vez que se dispara
      // y todas las balas posteriores reutilizan el mismo Quad en VRAM
      static std::shared_ptr<Mesh> bulletMesh = Mesh::CreateQuad();

      // 2. Crear la entidad en la escena
      auto* bullet = scene->CreateGameObject("Bullet");
      bullet->tag = "Bullet"; // 👈 Asignamos tag

      // 3. Transform con constructor variádico (X, Y, Ancho, Alto)
      bullet->AddComponent<Transform>(x, y, 24, 12);

      // 4. MeshRenderer: Malla compartida + Color Amarillo brillante (RGBA en 0.0f - 1.0f)
      bullet->AddComponent<MeshRenderer>(bulletMesh, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

      // 5. Componentes de vuelo y colisión
      bullet->AddComponent<Bullet>();
      bullet->AddComponent<BoxCollider>();

      return bullet;
    }
};