#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"

class Hero : public Component {
public:
    float speed = 250.0f;
    Transform* transform = nullptr; // Puntero cacheado

    // 1. Al nacer, busca su Transform una sola vez
    void Init() override {
        transform = gameObject->GetComponent<Transform>();
    }

    // 2. En cada fotograma, el Hero mueve su propia posición
    void Update(float deltaTime) override {
        if (transform != nullptr) {
            transform->x += speed * deltaTime;
            if (transform->x > 800.0f) {
                transform->x = -100.0f;
            }
        }
    }
};