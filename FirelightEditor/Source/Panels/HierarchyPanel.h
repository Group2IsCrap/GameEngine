#pragma once

#include <vector>

#include "../Includes/imgui/imgui.h"

namespace Firelight::ECS
{
	class Entity;
	class SpriteEntity;
}

class InspectorPanel;

class HierarchyPanel
{
public:
	HierarchyPanel();

	void Draw();
	void NewGameEntity();
	void NewCameraEntity();
	Firelight::ECS::SpriteEntity* NewSpriteEntity();
	void DeleteGameEntity(Firelight::ECS::Entity* entity);

private:
	void DrawEntityNode(Firelight::ECS::Entity* entity);

public:
	InspectorPanel* m_inspectorPanel = nullptr;
	std::vector<Firelight::ECS::Entity*> m_entitiesInScene;
private:
	Firelight::ECS::Entity* m_selectionContext;
};