#pragma once
#include "Character.h"


class Player : public Character
{
public:
	void Initialize(Model* model, const uint32_t textureHandle) override;
};

