#pragma once
#include <cstdint>

//当たり判定用フラグ
const uint32_t kCollisionAttributePlayer = 0b1;//~0001(1)
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;//~0010(2)