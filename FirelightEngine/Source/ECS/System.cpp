#include "System.h"

#include "ECSEvents.h"
#include "../Events/EventDispatcher.h"

namespace Firelight::ECS
{
	System::System()
	{
		Events::EventDispatcher::SubscribeFunction(Events::ECS::OnEntityCreatedEvent::sm_descriptor, std::bind(&System::UpdateEntityList, this));
		Events::EventDispatcher::SubscribeFunction(Events::ECS::OnEntityDestroyedEvent::sm_descriptor, std::bind(&System::UpdateEntityList, this));
	}
	System::~System()
	{
	}
	/// <summary>
	/// Returns a list of the maintained entities
	/// </summary>
	/// <returns></returns>
	std::vector<Entity*> System::GetEntities()
	{
		return m_entities;
	}
	/// <summary>
	/// Primary Update Loop
	/// </summary>
	/// <param name="dt"></param>
	void System::Update(double dt)
	{
	}
	/// <summary>
	/// Secondary Update Loop
	/// </summary>
	/// <param name="dt"></param>
	void System::LateUpdate(double dt)
	{
	}
	/// <summary>
	/// Fixed Update loop
	/// </summary>
	/// <param name="fixeddt"></param>
	void System::FixedUpdate(double fixeddt)
	{
	}
}