#pragma once

#include <Source/ECS/Systems/System.h>

#include "../Player/PlayerEntity.h"

class PlayerSystem : public Firelight::ECS::System
{
public:
	PlayerSystem();
	~PlayerSystem();
	void CheckForPlayer();
	void Update(const Firelight::Utils::Time& time) override;
private:
	size_t m_playerEntityAddedCheckIndex;
	size_t m_playerMoveUpIndex;
	size_t m_playerMoveLeftIndex;
	size_t m_playerMoveDownIndex;
	size_t m_playerMoveRightIndex;
	PlayerEntity* playerEntity;

	void MovePlayerUp();
	void MovePlayerLeft();
	void MovePlayerDown();
	void MovePlayerRight();
};