#pragma once

#include "Panel.h"

namespace Firelight::ECS
{
	class Entity;
}

class InspectorPanel : public Panel
{
public:
	InspectorPanel();

	void Draw() override;
	void DrawComponents(Firelight::ECS::Entity* entity);

public:
	Firelight::ECS::Entity* m_selectionContext;
};
