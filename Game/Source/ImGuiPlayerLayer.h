#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"
#include "Source/Engine.h"
#include "Source/ECS/EntityWrappers/Entity.h";


#include <vector>
#include <functional>

using CallbackFunctionType = std::function<void()>;

class ImGuiPlayerLayer : public Firelight::ImGuiUI::ImGuiLayer
{
public:
	void Render() override;
	Firelight::ECS::Entity* playerEntity;
public:

	ImGuiPlayerLayer();
	~ImGuiPlayerLayer();

	void SetPlayer(Firelight::ECS::Entity* entity);

	void RenderInformation();
private:
	void SetupTheme();

private:
};