#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"
#include "Source/Engine.h"


#include <vector>
#include <functional>

using CallbackFunctionType = std::function<void()>;

class ImGuiDebugLayer : public Firelight::ImGuiUI::ImGuiLayer
{
public:
	void Render() override;
	std::vector<CallbackFunctionType> spawnItemCommand;
public:

	ImGuiDebugLayer();
	~ImGuiDebugLayer();

	void Initialize();
private:
	void SetupTheme();
	void AddKeyBindingWidget(Firelight::KeyboardBindingData binding);
	void AddButtonBindingWidget(Firelight::ControllerBindingData binding);

	void RenderItemWindow();
	void RenderControls();
	void RenderECSDebug();
	void RenderKeyBindingPrototype();
	void RenderDebugInformation();
private:

	float m_currentFPS;
	float m_fpsUpdateDelay;
};