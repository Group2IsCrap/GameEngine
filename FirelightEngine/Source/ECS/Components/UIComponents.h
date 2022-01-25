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

		PixleSpriteComponent* Textuer = nullptr;
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

		//pos data
		e_AnchorSettings DockSettings = e_AnchorSettings::None;
		Maths::Vec3f DefaultPosition= Maths::Vec3f(0,0,0);
	};


	//hold all UI
	struct UI_Canvas : UIWidget
	{
		std::vector<UIWidget*> Child;
	};
	//Local Docking panel
	struct Panel : UIWidget
	{
		UIWidget* Parent;
		std::vector<UIWidget*> Child;
	};

	//Visable UI
	
	struct UI_Child : UIWidget 
	{
		UIWidget* Parent;
	};

	struct UI_Button : UI_Child
	{
		bool isChangeOfTex = true;
		Graphics::Colour::RGBA colour[3] = { Firelight::Graphics::Colours::sc_white ,Firelight::Graphics::Colours::sc_black,Firelight::Graphics::Colours::sc_defaultMetallic};
		std::vector<Maths::Rectf> Rects;
		char ButtionText;

	};
	struct UI_Immage : UI_Child
	{
		
	};
}