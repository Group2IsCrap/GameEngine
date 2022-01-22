#pragma once
#include <string>
#include "../ECSDefines.h"
#include"../Components/BasicComponents.h"
#include"..\ECS\Components\RenderingComponents.h"
#include<vector>
#include"..\Source\Events\Event.h"
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
	struct UIWidget
	{

		SpriteComponent* Textuer;
		TransformComponent* Transform = nullptr;

		std::string Name = "UI";
		UINT Index =0;

		//Events 
		bool isPressable = true;
		std::vector<CallbackFunctionType> OnLeftPressFunctions;
		std::vector<CallbackFunctionType> OnRightPressFunctions;
		std::vector<CallbackFunctionType> OnMiddlePressFunctions;
		bool isHover;
		std::vector<CallbackFunctionType> OnHoverFunctions;
		bool isDrag;
		
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