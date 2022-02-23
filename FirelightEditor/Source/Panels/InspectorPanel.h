#pragma once

#include "../Includes/imgui/imgui.h"
#include "../Includes/imgui/imgui_internal.h"

namespace Firelight::ECS
{
	class Entity;
}

class InspectorPanel
{
public:
	InspectorPanel();

	void Draw();
	void DrawComponents(Firelight::ECS::Entity* entity);

public:
	Firelight::ECS::Entity* m_selectionContext;
};
