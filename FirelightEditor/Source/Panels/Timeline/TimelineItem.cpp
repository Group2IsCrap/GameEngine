#include "TimelineItem.h"

#include <algorithm>

void TimelineItem::SetParent(TimelineItem* parent)
{
	m_parent = parent;
}

void TimelineItem::AddChild(TimelineItem* child)
{
	m_children.push_back(child);
}

void TimelineItem::RemoveChild(TimelineItem* child)
{
	if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	}
}
