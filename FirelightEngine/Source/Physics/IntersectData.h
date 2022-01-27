#pragma once

#include "../Source/Maths/Vec3.h"

namespace Firelight::Physics
{

	class IntersectData
	{
	public:
		IntersectData(const bool intersects, Firelight::Maths::Vec3f direction) : m_intersects(intersects), m_direction(direction) {}

		inline bool GetDoesIntersects() const { return m_intersects; }
		inline const Firelight::Maths::Vec3f GetDirection() const { return m_direction; }
		inline float GetDistance() const { m_direction.Length(); }

	private:
		Firelight::Maths::Vec3f m_direction;
		bool m_intersects;
	};

}
