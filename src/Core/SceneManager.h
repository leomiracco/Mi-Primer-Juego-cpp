#pragma once
#include <memory>
#include <functional>
#include <utility>
#include "Core/Scene.h"

class SceneManager {
  public:
    // Carga una escena nueva y guarda la receta para poder reiniciarla
    template <typename T, typename... Args>
    static void LoadScene(Args&&... args) {
      sceneCreator = [args...]() {
        return std::make_unique<T>(args...);
      };
      pendingScene = sceneCreator();
    }

    // 👉 NUEVO: Reinicia la escena activa sin importar cuál sea
    static void ReloadCurrentScene() {
      if (sceneCreator) {
        pendingScene = sceneCreator();
      }
    }

    static void Update(float deltaTime) {
      ApplyPendingScene();
      if (activeScene != nullptr) {
        activeScene->Update(deltaTime);
      }
    }

    static void Render() {
      if (activeScene != nullptr) {
        activeScene->Render();
      }
    }

    static void Clean() {
      activeScene.reset();
      pendingScene.reset();
      sceneCreator = nullptr;
    }

    static Scene* GetActiveScene() {
      return activeScene.get();
    }

  private:
    static inline std::unique_ptr<Scene> activeScene = nullptr;
    static inline std::unique_ptr<Scene> pendingScene = nullptr;
    // Guarda la función constructora de la escena activa (C++20)
    static inline std::function<std::unique_ptr<Scene>()> sceneCreator = nullptr;

    static void ApplyPendingScene() {
      if (pendingScene != nullptr) {
        activeScene = std::move(pendingScene);
        activeScene->Init();
      }
    }
};