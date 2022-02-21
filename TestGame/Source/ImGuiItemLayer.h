#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"

#include <vector>
#include <functional>

using CallbackFunctionType = std::function<void()>;

class ImGuiItemLayer : public Firelight::ImGuiUI::ImGuiLayer
{
public:
	void Render() override;
	std::vector<CallbackFunctionType> spawnItemCommand;
public:

	ImGuiItemLayer();
	~ImGuiItemLayer();

private:
	void SetupTheme();
private:
};