#pragma once

#include "GameEntity.h"
#include "../Components/RenderingComponents.h"
#include"../Components/UIComponents.h"
namespace Firelight::ECS
{
	class UIEntity : public GameEntity
	{

	public:
		UIEntity();

		PixelSpriteComponent* GetSpriteComponent();
		UIBaseWidgetComponent* GetWidgetComponent();

		void SetAnchorSettings(e_AnchorSettings settings);
		void SetDefaultPosition(Maths::Vec3f pos);
		void SetDefaultDimensions(Maths::Vec3f dimensions);
		void SetDefaultScale(Maths::Vec3f scale);
		void SetScale(Maths::Vec3f scale);
		void SetOffset(Maths::Vec2f offset);
		void SetParent(EntityID parentID);
	};

	typedef UIEntity UIPanel;
}
