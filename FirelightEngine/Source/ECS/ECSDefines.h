#pragma once

#include <cstdint>
#include <string>
#include <imgui.h>
#include <functional>
#include "../Maths/Vec2.h"
#include "../Maths/Vec3.h"
#include "../Maths/Rect.h"
#include "../Graphics/Data/Colour.h"

#define DEFINE_COMPONENT(structName, base) \
struct structName : base \
{ \
	static BaseComponent* CreateInstance() \
	{ \
		return new structName(); \
	} \
	\
	static void RegisterComponent() \
	{ \
		Firelight::ECS::componentInstanceMap.insert(std::make_pair(typeid(structName).name(), std::bind(&structName::CreateInstance))); \
	} 

namespace Firelight::ECS
{
	typedef std::uint32_t EntityID;
	typedef std::uint16_t ComponentTypeID;
	typedef size_t TypeHash;


	/// <summary>
	/// Base struct of all components
	/// </summary>
	struct BaseComponent
	{
		/// <summary>
		/// Serialises the component. DO NOT NEED TO PUT START OBJECT
		/// </summary>
		/// <returns></returns>
		virtual void Serialise()
		{
			return;
		}

		virtual BaseComponent* Clone()
		{
			return nullptr;
		}

		virtual void RenderDebugUI()
		{
			ImGui::Text("Base Component contains No Data");
		}

		static void ImGuiVariable(std::string varName, std::string data)
		{
			ImGui::Text((varName + ": " + data).c_str());
		}

		static void ImGuiText(std::string text)
		{
			ImGui::Text(text.c_str());
		}

		//static void ImGuiVariable(std::string varName, bool data)
		//{
		//	ImGuiVariable(varName, data ? "true" : "false");
		//}

		static void ImGuiVariable(std::string varName, int data)
		{
			ImGuiVariable(varName, std::to_string(data));
		}

		static void ImGuiVariable(std::string varName, float data)
		{
			ImGuiVariable(varName, std::to_string(data));
		}
		static void ImGuiVariable(std::string varName, Maths::Vec2f data)
		{
			ImGuiVariable(varName, std::to_string(data.x) + ", " + std::to_string(data.y));
		}

		static void ImGuiVariable(std::string varName, Maths::Vec3f data)
		{
			ImGuiVariable(varName, std::to_string(data.x) + ", " + std::to_string(data.y) + ", " + std::to_string(data.z));
		}

		static void ImGuiVariable(std::string varName, Maths::Rectf data)
		{
			ImGuiVariable(varName, std::to_string(data.w) + ", " + std::to_string(data.h) + ", " + std::to_string(data.x) + ", " + std::to_string(data.y));
		}
		static void ImGuiVariable(std::string varName, Graphics::Colour::RGBA data)
		{
			ImGuiVariable(varName, std::to_string(data.GetR()) + ", " + std::to_string(data.GetG()) + ", " + std::to_string(data.GetB()) + ", " + std::to_string(data.GetA()));
		}
	};

	static std::unordered_map<std::string, std::function<BaseComponent* ()>> componentInstanceMap{};
}
