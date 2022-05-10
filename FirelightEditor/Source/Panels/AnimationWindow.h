#pragma once

#include "../Includes/imgui/imgui.h"
#include "../FirelightEngine/Source/Animation/Animation.h"

class AnimationWindow
{
public:
	AnimationWindow();
	~AnimationWindow();

	void Draw();
	void DrawMenuBar(class ImGuiWindow* window);
	void CreateAnimation();

	void DrawAnimation();

	void SaveAnimation(const char* fileName, rapidjson::StringBuffer* json);
	void SaveFile(const char* animName);
	void GetAllAnimations();

public:
	bool m_isOpen;
private:
	Firelight::Animation::Animation* m_selectedAnimation;
	int* m_selectedAnimEnum;

	std::vector<Firelight::Animation::Animation*> m_animations;
	std::vector<std::string> m_animationNames;
	bool m_playAnimimation;

	float m_currentFrameTime;
	int m_currentFrameIndex;
};