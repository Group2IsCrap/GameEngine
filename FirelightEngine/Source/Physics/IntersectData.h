#pragma once

#include "../Source/Maths/Vec3.h"

namespace Firelight::Physics
{

	class IntersectData
	{
	public:
		IntersectData(const bool intersects, Firelight::Maths::Vec3f direction, float length) : m_intersects(intersects), m_direction(direction), m_length(length) {}

		inline bool GetDoesIntersects() const { return m_intersects; }
		inline const Firelight::Maths::Vec3f GetDirection() const { return m_direction; }
		inline float GetDistance() const { m_length; }

	private:
		Firelight::Maths::Vec3f m_direction;
		float m_length;
		bool m_intersects;
	};

}
