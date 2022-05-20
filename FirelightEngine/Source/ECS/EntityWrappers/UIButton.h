#pragma once
#include "UIEntity.h"

namespace Firelight::ECS
{
	class UIButton : public UIEntity
	{
	public:
		UIButton();

		UIButton(std::string name);

		void BindOnLeftPressed(CallbackFunctionType callback);
		void BindOnRightPressed(CallbackFunctionType callback);
		void BindOnMiddlePressed(CallbackFunctionType callback);
		void BindOnHovered(CallbackFunctionType callback);
		UIButtonComponent* GetButtonComponent();
	};

	class UIDraggableButton : public UIButton
	{
	public:
		UIDraggableButton();
	};
}