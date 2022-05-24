#pragma once

#include <Source/ECS/Systems/System.h>
#include <Source/ECS/Components/UIComponents.h>
#include <Source/Maths/Vec3.h>

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
	void FixedUpdate(const Firelight::Utils::Time& time) override;
	virtual void HandleEvents(DescriptorType event, void* data) override;

	void SwitchWeapon();
	void ToggleDebug();
	void AddHealth(void* amount);
private:
	float GetSpeed();

	void MovePlayerUp();
	void MovePlayerLeft();
	void MovePlayerDown();
	void MovePlayerRight();

	void MovePlayerUpRelease();
	void MovePlayerLeftRelease();
	void MovePlayerDownRelease();
	void MovePlayerRightRelease();

	void HandlePlayerAnimations();

	void Interact();
	void SpawnItem();
	void Attack();
	void RemoveHealth();
	

	void StartAttack();
	void StopAttack();
	void Respawn();

private:
	size_t m_playerEntityAddedCheckIndex;

	size_t m_playerMoveUpIndex;
	size_t m_playerMoveLeftIndex;
	size_t m_playerMoveDownIndex;
	size_t m_playerMoveRightIndex;
	size_t m_playerMoveUpReleaseIndex;
	size_t m_playerMoveLeftReleaseIndex;
	size_t m_playerMoveDownReleaseIndex;
	size_t m_playerMoveRightReleaseIndex;

	size_t m_interactionEventIndex;
	size_t m_spawnItemEventIndex;
	size_t m_attackIndex;
	size_t m_releaseAttackIndex;
	size_t m_respawnIndex;
  
	size_t m_removeHealthEventIndex;
	size_t m_addHealthEventIndex;

	Firelight::Maths::Vec3f m_velocity;
	PlayerEntity* playerEntity;
  
	bool m_moveUp;
	bool m_moveDown;
	bool m_moveLeft;
	bool m_moveRight;
	ImGuiPlayerLayer* imguiLayer = nullptr;
	bool m_drawDebugUI;

	bool m_isAttacking = false;
	float m_attackCooldown = 0.0f;
	float m_currentWeaponCooldown = 1.0f;
	bool m_canAttack = true;
};