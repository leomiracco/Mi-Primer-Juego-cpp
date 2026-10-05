#pragma once
#include "Core/Scene.h"
#include "Components/Transform.h"
#include "Components/SquareRenderer.h"
#include "Components/BoxCollider.h"
#include "Components/Bullet.h"

// 👈 Fábrica desacoplada: sabe cómo armar entidades prefabricadas
class Prefabs {
public:
    static GameObject* SpawnBullet(Scene* scene, float x, float y) {
        if (scene == nullptr) return nullptr;

        // 1. Le pedimos a la escena del jugador que cree la entidad
        auto* bullet = scene->CreateGameObject("Bullet");

        // 2. Coordenadas de salida
        auto* bulletTransform = bullet->AddComponent<Transform>();
        bulletTransform->position.x = x;
        bulletTransform->position.y = y;
        bulletTransform->width = 24;
        bulletTransform->height = 12;

        // 3. Aspecto visual (Amarillo)
        auto* bulletRender = bullet->AddComponent<SquareRenderer>();
        bulletRender->r = 255; bulletRender->g = 255; bulletRender->b = 0;

        // 4. Lógica de vuelo y cuerpo físico
        bullet->AddComponent<Bullet>();
        bullet->AddComponent<BoxCollider>();

        return bullet;
    }
};