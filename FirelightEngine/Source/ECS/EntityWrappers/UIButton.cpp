#include "UIButton.h"

namespace Firelight::ECS
{
	UIButton::UIButton() : UIEntity()
	{
		AddComponent<UIHoverableComponent>();
		AddComponent<UIPressableComponent>();
		AddComponent<UIButtonComponent>();
		AudioComponent* audioComponent = new AudioComponent();
		AddComponent<Firelight::ECS::AudioComponent>(audioComponent);
		audioComponent->looping = false;
		audioComponent->is3d = false;
		audioComponent->streaming = false;
		audioComponent->channel = "UI";
		audioComponent->soundName = "click1.wav";
		audioComponent->soundPos = Vector3D(0.0f, 0.0f, 0.0f);
		this->GetIDComponent()->name = "UI Button";
	}

	UIButton::UIButton(std::string name) : UIButton()
	{
		this->GetIDComponent()->name = name;
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