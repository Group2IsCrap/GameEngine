#include "UIButton.h"

namespace Firelight::ECS
{
	UIButton::UIButton()
	{
		AddComponent<UIHoverableComponent>();
		AddComponent<UIPressableComponent>();
		AddComponent<UIButtonComponent>();
	}

	void UIButton::BindOnLeftPressed(CallbackFunctionType callback)
	{
		GetComponent<UIPressableComponent>()->onLeftPressFunctions.push_back(callback);
	}

	void UIButton::BindOnRightPressed(CallbackFunctionType callback)
	{
		GetComponent<UIPressableComponent>()->onRightPressFunctions.push_back(callback);
	}

	void UIButton::BindOnMiddlePressed(CallbackFunctionType callback)
	{
		GetComponent<UIPressableComponent>()->onMiddlePressFunctions.push_back(callback);
	}

	void UIButton::BindOnHovered(CallbackFunctionType callback)
	{
		GetComponent<UIHoverableComponent>()->onHoverFunctions.push_back(callback);
	}

	UIButtonComponent* UIButton::GetButtonComponent()
	{
		return GetComponent<UIButtonComponent>();
	}

	UIDraggableButton::UIDraggableButton()
	{
		AddComponent<UIDraggableComponent>();
	}

}