#pragma once
#include  "Player.h"


class Enemy : public Character
{
public:
	void Initialize(Model* model, const uint32_t textureHandle) override;

};

