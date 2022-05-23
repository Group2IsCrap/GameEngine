#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../EntityWrappers/Entity.h"
#include "../../Maths/Vec3.h"
#include "../../Serialisation/Serialiser.h"
#include "TransformData.h"

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
		
		Firelight::Maths::Vec3f GetPosition() { return transformData->GetPosition(); }
		void SetPosition(const Firelight::Maths::Vec3f& pos) { transformData->SetPosition(pos); }

		float GetRotation() { return transformData->GetRotation(); }
		void SetRotation(float rot) { transformData->SetRotation(rot); };

		Firelight::Maths::Vec3f GetScale() { return transformData->GetScale(); }
		void SetScale(const Firelight::Maths::Vec3f& scale) { transformData->SetScale(scale); }

		void FlipX(bool flip, bool flipPos = true) { transformData->FlipX(flip, flipPos); }
		bool GetFlipped() { return transformData->GetFlipped(); }

		void SetParent(Entity* parent) { transformData->SetParent(parent); }
		void AddChild(Entity* child) { transformData->AddChild(child); }
		void RemoveChild(Entity* child) { transformData->RemoveChild(child); }
		Entity* GetChild(int index) { transformData->GetChild(index); }
		std::map<EntityID, Entity*> GetChildren() { transformData->GetChildren(); }

		void Serialise() override
		{
			Serialiser::Serialise("Rotation", GetRotation());
			Serialiser::Serialise("Position", GetPosition());
			Serialiser::Serialise("Scale", GetScale());
		}

		TransformComponent* Clone() override
		{
			TransformComponent* clone = new TransformComponent();
			clone->SetPosition(GetPosition());
			clone->transformData->SetRotation(GetRotation());
			clone->transformData->SetScale(GetScale());

			return clone;
		}


		void RenderDebugUI() override
		{
			ImGuiVariable("Position", GetPosition());
			ImGuiVariable("Rotation", GetRotation());
			ImGuiVariable("Scale", GetScale());
		}
	private:
		TransformData* transformData = new TransformData();
	};
}
