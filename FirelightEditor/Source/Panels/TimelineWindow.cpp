#include "TimelineWindow.h"

TimelineWindow::TimelineWindow()
{
	m_targetFPS = 30;
	m_isOpen = true;

	AddItem("Layer 1");
	AddItem("Layer 2");
	AddItem("Layer 3");
}

TimelineWindow::~TimelineWindow()
{
}

void TimelineWindow::Draw()
{
	ImGui::Begin("Timeline");
	
	float test[] = {0.0f, 1.0f };
	ImGui::PlotLines("Frames", test, IM_ARRAYSIZE(test));

	ImGui::End();
}

void TimelineWindow::AddItem(const char* name)
{
	m_items.push_back(new TimelineItem(name));
}
