#pragma once

#include "../ECSDefines.h"
#include "../EntityWrappers/Entity.h"
#include "../../Maths/Vec3.h"

namespace Firelight::ECS
{
	struct TransformData
	{
	public:
		Firelight::Maths::Vec3f GetPosition() { return position; }
		void SetPosition(const Firelight::Maths::Vec3f& pos);

		float GetRotation() { return rotation; }
		void SetRotation(float rot) { rotation = rot; }

		Firelight::Maths::Vec3f GetScale() { return scale; }
		void SetScale(const Firelight::Maths::Vec3f& scale) { this->scale = scale; }

		// TODO : Replace flip pos with binary search of root to not flip the root object if no children are present.
		void FlipX(bool flip, bool flipPos = true);

		bool GetFlipped() { return isFlipped; }

		void SetParent(Entity* parent);

		Entity* GetParent() { return parent; }

		void AddChild(Entity* child) { if (children.contains(child->GetEntityID())) return; children[child->GetEntityID()] = child; }
		void RemoveChild(Entity* child) { if (children.contains(child->GetEntityID())) children.erase(child->GetEntityID()); }
		Entity* GetChild(int index) { if (children[index] != nullptr) return children[index]; return nullptr; }
		std::map<EntityID, Entity*> GetChildren() { return children; }


		Firelight::Maths::Vec3f scale;
		float rotation = 0.0f;

	private:
		Firelight::Maths::Vec3f position;
		Entity* parent;
		std::map<EntityID, Entity*> children;
		bool isFlipped;
	};
}