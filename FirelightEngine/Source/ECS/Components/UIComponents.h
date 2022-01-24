#pragma once
#include <string>
#include "../ECSDefines.h"
#include"../Components/BasicComponents.h"
#include"RenderingComponents.h"
#include<vector>

namespace Firelight::ECS
{
	using CallbackFunctionType = std::function< void() >;
	//Docking Settings
	enum class e_DockSettings
	{
		DockRight,
		DockLeft,
		DockCenter,
		DockTop,
		DockBottom,
		DockNone

	};

	//Base Widget
	struct UIWidget : BaseComponent
	{

		SpriteComponent* Textuer;
		TransformComponent* Transform = nullptr;

		std::string Name = "UI";
		UINT Index =10;

		//Events 
		bool isPressable = true;
		std::vector<CallbackFunctionType> OnLeftPressFunctions;
		std::vector<CallbackFunctionType> OnRightPressFunctions;
		std::vector<CallbackFunctionType> OnMiddlePressFunctions;
		bool isHover =true;
		std::vector<CallbackFunctionType> OnHoverFunctions;
		bool isDrag =true;
		
		//Ui_Navergation
		bool isFocus = false;
		
		//DockSettings
		e_DockSettings DockSettings = e_DockSettings::DockNone;
	};


	//hold all UI
	struct UI_Canvas : UIWidget
	{
		bool hasChild = false;
		//Maths::Rectf Size;
		std::vector<UIWidget> Child;
		

	};
	//Local Docking panel
	struct Panel : UIWidget
	{
		UI_Canvas Parent;
		std::vector <UIWidget> Child;
		e_DockSettings DockSettings = e_DockSettings::DockNone;
		
	};

	//Visable UI
	struct UI_Button : UIWidget
	{
		UIWidget parent;
		bool isChanhgeTexRect;
		char text;
		e_DockSettings DockSettings = e_DockSettings::DockNone;
	};
	struct UI_Immage : UIWidget
	{
		UIWidget parent;
		e_DockSettings DockSettings = e_DockSettings::DockNone;
	};
}