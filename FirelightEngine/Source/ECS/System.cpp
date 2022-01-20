#include "System.h"

namespace Firelight::ECS
{
	System::System()
	{
	}
	System::~System()
	{
	}
	std::vector<Entity*> System::GetEntities()
	{
		return m_entities;
	}
	void System::Update(double dt)
	{
	}
	void System::LateUpdate(double dt)
	{
	}
	void System::FixedUpdate(double fixeddt)
	{
	}
}