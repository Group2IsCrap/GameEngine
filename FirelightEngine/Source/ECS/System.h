#pragma once

#include "Entity.h"

namespace Firelight::ECS
{
	class System
	{
	public:
		System();
		~System();
		std::vector<Entity*> GetEntities();
		virtual void Update(double dt);
		virtual void LateUpdate(double dt);
		virtual void FixedUpdate(double fixeddt);
	private:
	public:
	private:
		std::vector<Entity*> m_entities;
	};

}