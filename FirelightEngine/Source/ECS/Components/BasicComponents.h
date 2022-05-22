#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../EntityWrappers/Entity.h"
#include "../../Maths/Vec3.h"
#include "../../Serialisation/Serialiser.h"

#include <map>

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	/// <summary>
	/// Simple ID component used for testing
	/// </summary>
	
	DEFINE_COMPONENT(IdentificationComponent, BaseComponent)
		std::string name;

		void Serialise() override
		{
			Serialiser::Serialise("Name", name.c_str());
		}

		IdentificationComponent* Clone() override
		{
			IdentificationComponent* clone = new IdentificationComponent();
			clone->name = name;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Name", name);
		}
	};

	/// <summary>
	/// Stores whether an entity is considered static or not
	/// </summary>
	struct StaticComponent : BaseComponent
	{
		bool isStatic = false;

		void Serialise() override
		{
			Serialiser::Serialise("IsStatic", isStatic);
		}

		StaticComponent* Clone() override
		{
			StaticComponent* clone = new StaticComponent();
			clone->isStatic = isStatic;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("IsStatic", isStatic ? "true" : "false");
		}
	};

	/// <summary>
	/// Simple layer component
	/// </summary>
	struct LayerComponent : BaseComponent
	{
		// Layer 0 = Default
		int layer = 0;

		void Serialise() override
		{
			Serialiser::Serialise("Layer", layer);
		}

		LayerComponent* Clone() override
		{
			LayerComponent* clone = new LayerComponent();
			clone->layer = layer;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Layer", layer);
		}

	};

	/// <summary>
	/// Simple transform component used for testing
	/// </summary>
	struct TransformComponent : BaseComponent
	{
	public:
		Firelight::Maths::Vec3f GetPosition() { return position; }
		void SetPosition(const Firelight::Maths::Vec3f& pos) 
		{
			Firelight::Maths::Vec3f difference = pos - position;

			// Update child objects
			for (auto child : children)
			{
				TransformComponent* transform = child.second->GetComponent<TransformComponent>();
				transform->SetPosition(transform->GetPosition() + difference);
			}

			position = pos;
		}

		// TODO : Replace flip pos with binary search of root to not flip the root object if no children are present.
		void FlipX(bool flip, bool flipPos = true)
		{
			if (isFlipped == flip)
				return;

			// TODO : Binary search to root and flip basd on root. Currently,
			// root must be flipped or items will not work aka do not flip child objects unless it's the
			// top of the chain that you care for.
			for (auto child : children)
			{
				child.second->GetComponent<TransformComponent>()->FlipX(flip);
			}

			if (flipPos)
			{
				Firelight::Maths::Vec3f inverted = Firelight::Maths::Vec3f(position.x *= -1, position.y, position.z);
				SetPosition(inverted);
			}
			isFlipped = flip;
		}

		bool GetFlipped() { return isFlipped; }

		void SetParent(Entity* parent) 
		{ 
			if (children.contains(parent->GetEntityID()))
			{
				return;
			}
			this->parent = parent;
		}
		Entity* GetParent() { return parent; }

		void AddChild(Entity* child) { if (children.contains(child->GetEntityID())) return; children[child->GetEntityID()] = child; }
		void RemoveChild(Entity* child) { if (children.contains(child->GetEntityID())) children.erase(child->GetEntityID()); }
		Entity* GetChild(int index) { if (children[index] != nullptr) return children[index]; return nullptr; }
		std::map<EntityID, Entity*> GetChildren() { return children; }
		

		Firelight::Maths::Vec3f scale;
		float rotation = 0.0f;

		void Serialise() override
		{
			Serialiser::Serialise("Rotation", rotation);
			Serialiser::Serialise("Position", position);
			Serialiser::Serialise("Scale", scale);
		}

		TransformComponent* Clone() override
		{
			TransformComponent* clone = new TransformComponent();
			clone->position = position;
			clone->scale = scale;
			clone->rotation = rotation;

			return clone;
		}


		void RenderDebugUI() override
		{
			ImGuiVariable("Position", position);
			ImGuiVariable("Rotation", rotation);
			ImGuiVariable("Scale", scale);
		}

	private:
		Firelight::Maths::Vec3f position;
		Entity* parent;
		std::map<EntityID, Entity*> children;
		bool isFlipped;

	};

}
