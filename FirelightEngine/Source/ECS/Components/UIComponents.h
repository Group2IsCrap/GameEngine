#pragma once
#include <string>
#include "../ECSDefines.h"
#include"../Components/BasicComponents.h"
#include"RenderingComponents.h"
#include "../../Serialisation/Serialiser.h"
#include<vector>
#include <functional>

using namespace Firelight::Serialisation;

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
	struct UIBaseWidgetComponent : BaseComponent
	{
		EntityID parentID = 0;
		bool hasParent = false;

		UINT index = 10;

		e_AnchorSettings anchorSettings = e_AnchorSettings::None;
		Maths::Vec2f offSet = (0, 0);

		Maths::Vec3f defaultPosition = Maths::Vec3f(0, 0, 0);
		Maths::Vec3f defaultDimensions = Maths::Vec3f(100, 100, 0);
		Maths::Vec3f defaultScale = Maths::Vec3f(1, 1, 0);
		Maths::Vec3f currentScale = Maths::Vec3f(1, 1, 0);

		void Serialise() override
		{
			Serialiser::Serialise("ParentID",  parentID);
			Serialiser::Serialise("HasParent", hasParent);
			Serialiser::Serialise("Index", index);

			//Maybe?
			Serialiser::Serialise("AnchorSettings", (int)anchorSettings);

			Serialiser::Serialise("Offset", offSet);
			Serialiser::Serialise("DefaultPosition", defaultPosition);
			Serialiser::Serialise("DefaultDimensions", defaultDimensions);
			Serialiser::Serialise("DefaultScale", defaultScale);
			Serialiser::Serialise("CurrentScale", currentScale);
		}
	};

	struct UIPressableComponent : BaseComponent
	{
		std::vector<CallbackFunctionType> onLeftPressFunctions;
		std::vector<CallbackFunctionType> onRightPressFunctions;
		std::vector<CallbackFunctionType> onMiddlePressFunctions;
	};
	struct UIHoverableComponent : BaseComponent
	{
		std::vector<CallbackFunctionType> onHoverFunctions;
	};

	struct UIDraggableComponent : BaseComponent
	{

	};

	struct UIContainerComponent : BaseComponent
	{

	};

	struct UICanvasComponent : BaseComponent
	{
		float XScreenSize;
		float YScreenSize;
		float layer = 32;

		void Serialise() override
		{
			Serialiser::Serialise("Layer", layer);
			Serialiser::Serialise("XScreenSize", XScreenSize);
			Serialiser::Serialise("YScreenSize", YScreenSize);
		}
	};

	struct UIButtonComponent : BaseComponent
	{
		bool isChangeOfTex = true;
		Graphics::Colour::RGBA colour[3] = { Firelight::Graphics::Colours::sc_white ,Firelight::Graphics::Colours::sc_black,Firelight::Graphics::Colours::sc_defaultMetallic};
		std::vector<Maths::Rectf> rectsOfButton;
		const char* buttonText = nullptr;

		void Serialise() override
		{
			Serialiser::Serialise("IsChangeOfTex", isChangeOfTex);

			Serialiser::StartArray("Colours");
			Serialiser::Serialise("Colour", colour[0]);
			Serialiser::Serialise("Colour", colour[1]);
			Serialiser::Serialise("Colour", colour[2]);
			Serialiser::EndArray();

			Serialiser::StartArray("RectsOfButton");
			for (int i = 0; i < rectsOfButton.size(); ++i)
			{
				Serialiser::Serialise("Rect", rectsOfButton[i]);
			}
			Serialiser::EndArray();

			Serialiser::Serialise("ButtonText", buttonText);
		}
	};


	////Base Widget
	//struct UIWidgetComponent : BaseComponent
	//{
	//	/*PixelSpriteComponent* texture = nullptr;
	//	TransformComponent* transform = nullptr;*/

	//	UINT index = 10;
 //   
	//	//Events 
	//	bool isPressable = false;
	//	std::vector<CallbackFunctionType> onLeftPressFunctions;
	//	std::vector<CallbackFunctionType> onRightPressFunctions;
	//	std::vector<CallbackFunctionType> onMiddlePressFunctions;
	//	bool isHoverable = false;
	//	std::vector<CallbackFunctionType> onHoverFunctions;
	//	bool isDraggable = false;

	//	//pos data
	//	e_AnchorSettings anchorSettings = e_AnchorSettings::None;
	//	Maths::Vec2f offSet = (0, 0);

	//	//size elments
	//	Maths::Vec3f defaultPosition= Maths::Vec3f(0,0,0);
	//	Maths::Vec3f defaultSize = Maths::Vec3f(100, 100, 0);
	//	Maths::Vec3f defaultScale = Maths::Vec3f(1, 1, 0);
	//	Maths::Vec3f currentScale = Maths::Vec3f(1, 1, 0);

	//	void Serialise() override
	//	{
	//		Serialiser::Serialise("Index", index);
	//		Serialiser::Serialise("IsPressable", isPressable);

	//		// Serialise Callbacks

	//		Serialiser::Serialise("IsHoverable", isHoverable);
	//		Serialiser::Serialise("IsDraggable", isDraggable);

	//		//Maybe?
	//		Serialiser::Serialise("AnchorSettings", (int)anchorSettings);

	//		Serialiser::Serialise("Offset", offSet);
	//		Serialiser::Serialise("DefaultPosition", defaultPosition);
	//		Serialiser::Serialise("DefaultScale", defaultScale);
	//		Serialiser::Serialise("CurrentScale", currentScale);
	//	}
	//};

	////Visable UI
	//struct UIChildComponent : UIBaseWidgetComponent
	//{
	//	EntityID parent;

	//	void Serialise() override
	//	{
	//		UIWidgetComponent::Serialise();
	//		Serialiser::Serialise("ParentID", parent);
	//	}
	//};
	// 
	////Local ancoring  panel
	//struct UIPanelComponent : UIBaseWidgetComponent
	//{
	//	void Serialise() override
	//	{
	//		UIBaseWidgetComponent::Serialise();
	//	}
	//};

	//struct UIButtonComponent : UIChildComponent
	//{
	//	UIButtonComponent()
	//	{
	//		isDraggable = true;
	//		isPressable = true;
	//		isHoverable = true;
	//	}

	//	bool isChangeOfTex = true;
	//	Graphics::Colour::RGBA colour[3] = { Firelight::Graphics::Colours::sc_white ,Firelight::Graphics::Colours::sc_black,Firelight::Graphics::Colours::sc_defaultMetallic};
	//	std::vector<Maths::Rectf> rectsOfButton;
	//	const char* buttonText = nullptr;

	//	void Serialise() override
	//	{
	//		UIChildComponent::Serialise();

	//		Serialiser::Serialise("IsChangeOfTex", isChangeOfTex);

	//		Serialiser::StartArray("Colours");
	//		Serialiser::Serialise("Colour", colour[0]);
	//		Serialiser::Serialise("Colour", colour[1]);
	//		Serialiser::Serialise("Colour", colour[2]);
	//		Serialiser::EndArray();

	//		Serialiser::StartArray("RectsOfButton");
	//		for (int i = 0; i < rectsOfButton.size(); ++i)
	//		{
	//			Serialiser::Serialise("Rect", rectsOfButton[i]);
	//		}
	//		Serialiser::EndArray();

	//		Serialiser::Serialise("ButtonText", buttonText);
	//	}
	//};
	//struct UIImageComponent : UIChildComponent
	//{
	//	void Serialise() override
	//	{
	//		UIChildComponent::Serialise();
	//	}
	//};


}