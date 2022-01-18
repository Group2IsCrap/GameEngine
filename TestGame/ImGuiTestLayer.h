#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"

class ImGuiTestLayer : public Firelight::ImGuiUI::ImGuiLayer
{
public:
	void Render() override;
};

