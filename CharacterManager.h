#pragma once
#include"Enemy.h"

class CharacterManager
{
private:
	Player* player = nullptr;
	Enemy* enemy = nullptr;


public:
	void Initialize(Player* player, Enemy* enemy);

	void Update();
	void Draw(const ViewProjection& view);

};

