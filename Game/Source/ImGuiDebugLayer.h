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

	void RenderItemWindow();

	void RenderKeyBindingPrototype();

	void RenderDebugInformation();
private:
	void SetupTheme();
	void AddKeyBindingWidget(Firelight::KeyboardBindingData binding);
	void AddButtonBindingWidget(Firelight::ControllerBindingData binding);

private:
};