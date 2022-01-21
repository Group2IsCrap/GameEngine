#pragma once
#include <string>
#include "../ECSDefines.h"
#include<vector>
namespace Firelight::ECS
{
	//Base Widget
	struct UIWidget
	{
		std::string Name;
		UINT Index;

		//CanEvents 
		bool isPressable;
		bool isHover;
		bool isDrag;

		//Ui_Navergation
		bool isFocus;
		
		Maths::Vec2f pos;
	};


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