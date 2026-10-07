#pragma once
#include "ECS/Component.h"
#include "Core/Input.h"
#include "Core/Time.h"
#include "Components/Camera.h"
#include "Core/SceneManager.h"

class GameManager : public Component {
  public:
    void Update(float deltaTime) override {
      // 1. REINICIO GLOBAL: Sigue funcionando aunque el jugador muera
      if (Input::GetKeyDown(KeyCode::R)) {
        SceneManager::ReloadCurrentScene();
        return;
      }

      // ⏸️ 2. PAUSA GLOBAL
      if (Input::GetKeyDown(KeyCode::P)) {
        if (Time::IsPaused()) {
          Time::Resume();
        } else {
          Time::Pause();
        }
      }

      // ⏳ 3. CÁMARA LENTA (Slow-motion debug)
      if (Input::GetKeyDown(KeyCode::T)) {
        if (Time::GetTimeScale() < 1.0f) {
          Time::SetTimeScale(1.0f);
        } else {
          Time::SetTimeScale(0.2f);
        }
      }

      // 🔍 Zoom con rueda usando Camera::GetMain()
      float scroll = Input::GetMouseScroll();
      if (scroll != 0.0f) {
        Camera* cam = Camera::GetMain();
        if (cam != nullptr) {
          cam->zoom = std::clamp(cam->zoom + (scroll * 0.1f), 0.2f, 3.0f);
        }
      }
    }
};