#pragma once

#include "../Includes/imgui/imgui.h"
#include "../FirelightEngine/Source/Animation/Animation.h"
#include "Timeline/TimelineItem.h"

#include <vector>

class TimelineWindow
{
public:
	TimelineWindow();
	~TimelineWindow();

	void Draw();

private:
	void AddItem(const char* name);

public:
	bool m_isOpen;
private:
	int m_targetFPS;
	std::vector<TimelineItem*> m_items;
};