#pragma once

#include <string>
#include <vector>

struct TimelineItem
{
public:
	TimelineItem(const char* name) : m_itemName(name), m_parent(nullptr) {  }
	~TimelineItem() {}

	void SetParent(TimelineItem* parent);
	void AddChild(TimelineItem* child);
	void RemoveChild(TimelineItem* child);

public:
	TimelineItem* m_parent;
	std::vector<TimelineItem*> m_children;

public:
	std::string m_itemName;
};
