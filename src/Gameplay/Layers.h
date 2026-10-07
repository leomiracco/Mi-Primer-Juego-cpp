#pragma once
#include <cstdint>

namespace Layer {
    constexpr uint32_t Default      = 1 << 0; // 1
    constexpr uint32_t Player       = 1 << 1; // 2
    constexpr uint32_t Enemy        = 1 << 2; // 4
    constexpr uint32_t PlayerBullet = 1 << 3; // 8
    constexpr uint32_t EnemyBullet  = 1 << 4; // 16
    constexpr uint32_t All          = 0xFFFFFFFF;
}