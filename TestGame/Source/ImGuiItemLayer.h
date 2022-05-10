#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"

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

private:
	void SetupTheme();
private:
};