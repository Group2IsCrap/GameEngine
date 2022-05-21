#pragma once

#include "../ECSDefines.h"
#include "../Source/Animation/Animation.h"
#include "../Source/Graphics/Data/Texture.h"
#include "../Source/Maths/Rect.h"

#include "../../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

#include <string>
#include <map>
#include <vector>

namespace Firelight::ECS
{
	struct AnimationComponent : BaseComponent
	{
		Firelight::Animation::Animation* currentAnimation = nullptr;
		float currentFrameCount = 0;
		int currentFrameIndex = 0;
		bool playOnStart = true;
		bool shouldPlay = false;
		std::map<std::string, Firelight::Animation::Animation> animations;

		void Serialise() override
		{
			Serialiser::Serialise("CurrentAnimation", currentAnimation);
			Serialiser::Serialise("CurrentFrameCount", currentFrameCount);
			Serialiser::Serialise("CurrentFrameIndex", currentFrameIndex);
			Serialiser::Serialise("PlayOnStart", playOnStart);
			Serialiser::Serialise("ShouldPlay", shouldPlay);

			//Serialise Map
		}

		AnimationComponent* Clone() override
		{
			AnimationComponent* clone = new AnimationComponent();
			clone->currentAnimation = currentAnimation;
			clone->currentFrameCount = currentFrameCount;
			clone->currentFrameIndex = currentFrameIndex;
			clone->playOnStart = playOnStart;
			clone->shouldPlay = shouldPlay;
			clone->animations = animations;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("CurrentAnimation", currentAnimation != nullptr ? currentAnimation->m_animationName : "nullptr");
			ImGuiVariable("CurrentFrameCount", currentFrameCount);
			ImGuiVariable("CurrentFrameIndex", currentFrameIndex);
			ImGuiVariable("PlayOnStart", playOnStart);
			ImGuiVariable("ShouldPlay", shouldPlay);
			ImGuiVariable("Animations", (int)animations.size());
		}
	};

	struct Bone
	{
	public:
		Bone* parent;
		std::vector<Bone*> children;

		void Rotate(float rotation);
		void SetPosition(Firelight::Maths::Vec3f& position)
		{
			this->position = position;
		}
		void SetAnchorPoint(Firelight::Maths::Vec3f& anchorPoint)
		{
			this->anchorPoint = anchorPoint;
		}

		void FlipX(bool flip)
		{
			if (isFlipped == flip)
				return;

			// TODO : Binary search to root and flip basd on root. Currently,
			// root must be flipped or items will not work aka do not flip child objects unless it's the
			// top of the chain that you care for.
			for (auto child : children)
			{
				child->FlipX(flip);
			}

			Firelight::Maths::Vec3f inverted = Firelight::Maths::Vec3f(position.x *= -1, position.y, position.z);
			SetPosition(inverted);
			isFlipped = flip;
		}

		Firelight::Maths::Vec3f& GetPosition() { return position; }
		Firelight::Maths::Vec3f& GetAnchorPoint() { return anchorPoint; }
		float& GetRotation() { return rotation; }

	private:
		Firelight::Maths::Vec3f position;
		Firelight::Maths::Vec3f anchorPoint;
		float rotation;
		bool isFlipped;
	};

	struct BoneSkeletonComponent : BaseComponent
	{
		Bone* rootBone;

		void AddNewBone(Bone* parent)
		{
			
		}
		
	};
}

