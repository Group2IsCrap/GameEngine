#pragma once
#include <string>
#include "../ECSDefines.h"
#include "../Components/BasicComponents.h"
#include "RenderingComponents.h"
#include "../../Serialisation/Serialiser.h"
#include <vector>
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
	enum class e_Scale
	{
		Absolute=0,
		Relative
	};

	//Base Widget
	struct UIBaseWidgetComponent : BaseComponent
	{
		EntityID parentID = 0;
		bool hasParent = false;
		bool isActive = true;

		UINT index = 10;

		e_AnchorSettings anchorSettings = e_AnchorSettings::None;
		Maths::Vec2f offSet = { 0, 0 };

		Maths::Vec3f defaultPosition = Maths::Vec3f(0, 0, 0);
		Maths::Vec3f defaultDimensions = Maths::Vec3f(100, 100, 0);

		e_Scale scaleSetting = e_Scale::Relative;
		Maths::Vec3f defaultScale = Maths::Vec3f(1, 1, 0);
		Maths::Vec3f currentScale = Maths::Vec3f(1, 1, 0);

		void Serialise() override
		{
			Serialiser::Serialise("ParentID",  parentID);
			Serialiser::Serialise("HasParent", hasParent);
			Serialiser::Serialise("Index", index);

			//Maybe?
			Serialiser::Serialise("AnchorSettings", (int)anchorSettings);
			Serialiser::Serialise("ScaleSetting", (int)scaleSetting);


			Serialiser::Serialise("Offset", offSet);
			Serialiser::Serialise("DefaultPosition", defaultPosition);
			Serialiser::Serialise("DefaultDimensions", defaultDimensions);
			Serialiser::Serialise("DefaultScale", defaultScale);
			Serialiser::Serialise("CurrentScale", currentScale);
		}


		void RenderDebugUI() override
		{
			ImGuiVariable("ParentID", (int)parentID);
			ImGuiVariable("HasParent", hasParent);
			ImGuiVariable("Index", (int)index);

			ImGuiVariable("AnchorSettings", (int)anchorSettings);
			ImGuiVariable("ScaleSetting", (int)scaleSetting);

			ImGuiVariable("Offset", offSet);
			ImGuiVariable("DefaultPosition", defaultPosition);
			ImGuiVariable("DefaultDimensions", defaultDimensions);
			ImGuiVariable("DefaultScale", defaultScale);
			ImGuiVariable("CurrentScale", currentScale);
		}
	};

	struct UIPressableComponent : BaseComponent
	{
		std::vector<CallbackFunctionType> onLeftPressFunctions;
		std::vector<CallbackFunctionType> onRightPressFunctions;
		std::vector<CallbackFunctionType> onMiddlePressFunctions;

		void RenderDebugUI() override
		{
			ImGuiVariable("OnLeftPressFunctions", (int)onLeftPressFunctions.size());
			ImGuiVariable("OnRightPressFunctions", (int)onRightPressFunctions.size());
			ImGuiVariable("OnMiddlePressFunctions", (int)onMiddlePressFunctions.size());
		}

	};

	struct UIHoverableComponent : BaseComponent
	{
		std::vector<CallbackFunctionType> onHoverFunctions;

		void RenderDebugUI() override
		{
			ImGuiVariable("OnHoverFunctions", (int)onHoverFunctions.size());
		}
	};

	struct UIDraggableComponent : BaseComponent
	{
		std::vector<CallbackFunctionType> onPickUpFunctions;
		std::vector<CallbackFunctionType> onDropFunctions;

		void RenderDebugUI() override
		{
			ImGuiVariable("OnPickUpFunctions", (int)onPickUpFunctions.size());
			ImGuiVariable("OnDropFunctions", (int)onDropFunctions.size());
		}
	};

	struct UIContainerComponent : BaseComponent
	{
		void RenderDebugUI() override
		{
			ImGuiText("UI Container Component Contains No Data");
		}
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

		void RenderDebugUI() override
		{
			ImGuiVariable("Layer", layer);
			ImGuiVariable("XScreenSize", XScreenSize);
			ImGuiVariable("YScreenSize", YScreenSize);
		}
	};

	struct UIButtonComponent : BaseComponent
	{
		bool isChangeOfTex = true;
		Graphics::Colour::RGBA colour[3] = { Firelight::Graphics::Colours::sc_white ,Firelight::Graphics::Colours::sc_black,Firelight::Graphics::Colours::sc_defaultMetallic};
		std::vector<Maths::Rectf> rectsOfButton;
		std::string buttonText = std::string();

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

			Serialiser::Serialise("ButtonText", buttonText.c_str());
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("IsChangeOfTex", isChangeOfTex);

			ImGuiText("Colours");
			ImGuiVariable("    Colour 1", colour[0]);
			ImGuiVariable("    Colour 2", colour[1]);
			ImGuiVariable("    Colour 3", colour[2]);

			ImGuiText("Rects of Button");
			for (int i = 0; i < rectsOfButton.size(); ++i)
			{
				ImGuiVariable("    Rect" + std::to_string(i), rectsOfButton[i]);
			}

			ImGuiVariable("ButtonText", buttonText);
		}
	};

	struct UIBorderComponent : BaseComponent
	{
		//Graphics::Colour::RGBA colour= Firelight::Graphics::Colours::sc_white;
		float widthTopBot;
		float widthLeftRight;

		void Serialise() override
		{
			Serialiser::Serialise("widthTopBot", widthTopBot);
			Serialiser::Serialise("widthLeftRight", widthLeftRight);
		}

		void RenderDebugUI() override
		{

			ImGuiVariable("widthTopBot", widthTopBot);
			ImGuiVariable("widthLeftRight", widthLeftRight);
		}
	};

}