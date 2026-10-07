#pragma once
#include <algorithm>
#include "ECS/Component.h"
#include "ECS/GameObject.h"

class Health : public Component {
  public:
    int maxHealth = 1;
    int currentHealth = 1;

    Health() = default;
    Health(int maxHp) : maxHealth(maxHp), currentHealth(maxHp) {}

    void Init() override {
      currentHealth = maxHealth;
      isDead = false;
    }

    // 👉 GUARDA DE MUERTE: Si ya murió, ignora daño redundante en el mismo frame
    void TakeDamage(int amount) {
      if (isDead) return;

      currentHealth -= amount;

      if (currentHealth <= 0) {
        currentHealth = 0;
        isDead = true;
        Die();
      }
    }

    bool IsDead() const {
      return isDead;
    }

  private:
    bool isDead = false; // 👈 Evita doble muerte, doble loot o doble explosión

    void Die() {
      gameObject->Destroy();
    }
};