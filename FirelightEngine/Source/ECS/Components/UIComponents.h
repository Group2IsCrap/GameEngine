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
	enum class e_AnchorSettings
	{
		Right,
		TopRight,
		BottomRight,
		Left,
		TopLeft,
		BottomLeft,
		Center,
		Top,
		Bottom,
		None

	};

	//Base Widget
	struct UIWidget : BaseComponent
	{

		NDCSpriteComponent* Textuer = nullptr;
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
		
		//DockSettings
		e_AnchorSettings DockSettings = e_AnchorSettings::None;
		UIWidget* Parent;
		std::vector<UIWidget*> Child;


		Maths::Vec3f DefaultPosition= Maths::Vec3f(0,0,0);
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