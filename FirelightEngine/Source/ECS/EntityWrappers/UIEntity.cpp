#include "UIEntity.h"

namespace Firelight::ECS
{
	UIEntity::UIEntity() : GameEntity()
	{
		AddComponent<UIBaseWidgetComponent>();
		AddComponent<PixelSpriteComponent>();
		this->GetIDComponent()->name = "UI Entity";
	}

	UIEntity::UIEntity(std::string name) : UIEntity()
	{
		this->GetIDComponent()->name = name;
	}

	PixelSpriteComponent* UIEntity::GetSpriteComponent()
	{
		return GetComponent<PixelSpriteComponent>();
	}

	UIBaseWidgetComponent* UIEntity::GetWidgetComponent()
	{
		return GetComponent<UIBaseWidgetComponent>();
	}

	void UIEntity::SetAnchorSettings(e_AnchorSettings settings)
	{
		GetComponent<UIBaseWidgetComponent>()->anchorSettings = settings;
	}
	
	void UIEntity::SetScaleSettings(e_Scale settings)
	{
		GetComponent<UIBaseWidgetComponent>()->scaleSetting = settings;
	}

	void UIEntity::SetDefaultPosition(Maths::Vec3f pos)
	{
		GetComponent<UIBaseWidgetComponent>()->defaultPosition = pos;
	}

	void UIEntity::SetDefaultDimensions(Maths::Vec3f dimensions)
	{
		GetComponent<UIBaseWidgetComponent>()->defaultDimensions = dimensions;
	}

	void UIEntity::SetDefaultScale(Maths::Vec3f scale)
	{
		GetComponent<UIBaseWidgetComponent>()->defaultScale = scale;
	}

	void UIEntity::SetScale(Maths::Vec3f scale)
	{
		GetComponent<UIBaseWidgetComponent>()->currentScale = scale;
	}

	void UIEntity::SetOffset(Maths::Vec2f offset)
	{
		GetComponent<UIBaseWidgetComponent>()->offSet = offset;
	}

	void UIEntity::SetParent(EntityID parentID)
	{
		GetComponent<UIBaseWidgetComponent>()->parentID = parentID;
		GetComponent<UIBaseWidgetComponent>()->hasParent = true;
	}
	
}