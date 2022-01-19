#pragma once

#include "Entity.h"

#include <iostream>

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

		virtual void UpdateEntityList() 
		{
			std::cout << "Here I am" << std::endl;
		};
	private:
	public:
	private:
		std::vector<Entity*> m_entities;
	};
}