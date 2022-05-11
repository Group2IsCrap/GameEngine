#pragma once

#include <Source/ECS/Systems/System.h>

#include "../Player/PlayerEntity.h"
#include "../Events/InputEvents.h"
#include "../ImGuiPlayerLayer.h"

using DescriptorType = const char*;

class PlayerSystem : public Firelight::ECS::System, Firelight::Events::Listener
{
public:
	PlayerSystem();
	~PlayerSystem();
	void CheckForPlayer();
	void Update(const Firelight::Utils::Time& time) override;
	virtual void HandleEvents(DescriptorType event, void* data) override;
private:
	size_t m_playerEntityAddedCheckIndex;
	size_t m_playerMoveUpIndex;
	size_t m_playerMoveLeftIndex;
	size_t m_playerMoveDownIndex;
	size_t m_playerMoveRightIndex;
	size_t m_interactionEventIndex;
	size_t m_spawnItemEventIndex;
  
	PlayerEntity* playerEntity;

	ImGuiPlayerLayer* imguiLayer = nullptr;

	void MovePlayerUp();
	void MovePlayerLeft();
	void MovePlayerDown();
	void MovePlayerRight();
	void Interact();
	void SpawnItem();
};