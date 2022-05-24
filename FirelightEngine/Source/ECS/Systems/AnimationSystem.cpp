#include "AnimationSystem.h"

#include "../Components/AnimationComponent.h"
#include "../Components/RenderingComponents.h"
#include "../Components/BasicComponents.h"
#include <filesystem>
#include "../../Graphics/AssetManager.h"
#include "../Source/Animation/Animation.h"

namespace Firelight::ECS
{
	AnimationSystem* AnimationSystem::sm_instance = nullptr;

	AnimationSystem* AnimationSystem::Instance()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new AnimationSystem();
			sm_instance->GetAllAnimations();
		}
		return sm_instance;
	}

	void AnimationSystem::GetAllAnimations()
	{
		int count = 1;
		for (auto& directoryEntry : std::filesystem::directory_iterator("Assets/Animations"))
		{
			const auto& path = directoryEntry.path();

			auto relativePath = std::filesystem::relative(path, "Assets/Animations");
			std::string filenameString = relativePath.filename().string();

			std::string numberString = std::to_string(count);
			const char* itemNumber = numberString.c_str();

			Firelight::Graphics::Texture* icon;

			std::string extension = path.extension().string();
			if (extension == ".anim")
			{
				std::string filePath = path.string();
				// Load file and create new Animation struct
				if (Serialiser::LoadFile(path.string().c_str()))
				{
					Firelight::Animation::Animation* animation = new Firelight::Animation::Animation();
					Serialiser::Deserialise("AnimationName", animation->m_animationName);
					Serialiser::Deserialise("Loop", animation->m_loop);
					Serialiser::Deserialise("FrameTime", animation->m_frameTime);
					Serialiser::Deserialise("FrameCount", animation->m_frameCount);
					auto genericArray = (*Serialiser::FileDocument)["TextureNames"].GetArray();
					for (auto& value : genericArray)
					{
						std::string string = value.GetString();;
						animation->m_textureNames.push_back(string);
						animation->m_textures.push_back(Firelight::Graphics::AssetManager::Instance().GetTexture(string));
					}
					m_animations.insert(std::pair<std::string, Firelight::Animation::Animation*>(animation->m_animationName, animation));
				}
			}
		}
	}

	AnimationSystem::AnimationSystem()
	{
		AddWhitelistComponent<Firelight::ECS::AnimationComponent>();
	}
	
	AnimationSystem::~AnimationSystem()
	{

	}
	
	void AnimationSystem::Update(const Utils::Time& time, const bool& isPaused)
	{
		if (!isPaused)
		{
			if (m_entities.size() <= 0)
			{
				return;
			}

			for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
			{
				auto* spriteComponent = m_entities[entityIndex]->GetComponent<SpriteComponent>();
				auto* animatorComponent = m_entities[entityIndex]->GetComponent<AnimationComponent>();

				if (spriteComponent == nullptr || animatorComponent == nullptr)
				{
					continue;
				}

				if (animatorComponent->currentAnimation == nullptr || !animatorComponent->shouldPlay)
				{
					continue;
				}

				Firelight::Animation::Animation* currentAnimation = animatorComponent->currentAnimation;

				if (animatorComponent->currentFrameIndex > currentAnimation->m_frameCount)
				{
					if (!currentAnimation->m_loop)
					{
						continue;
					}
				}

				if (currentAnimation == nullptr || currentAnimation->m_textures.size() < 1)
					return;


				animatorComponent->currentFrameCount += (float)time.GetDeltaTime() * 1000.0f;

				if (animatorComponent->currentFrameCount > currentAnimation->m_frameTime)
				{
					if (!currentAnimation->m_loop && animatorComponent->currentFrameIndex < currentAnimation->m_frameCount - 1 || currentAnimation->m_loop)
						animatorComponent->currentFrameIndex++;

					if (animatorComponent->currentFrameIndex >= currentAnimation->m_frameCount && currentAnimation->m_loop)
					{
						animatorComponent->currentFrameIndex = 0;
					}
					animatorComponent->currentFrameCount = 0.0f;
				}

				Firelight::Graphics::Texture* texture = currentAnimation->m_textures[animatorComponent->currentFrameIndex];
				if (texture == nullptr)
					return;

				Firelight::Maths::Vec3i imageDimensions = texture->GetDimensions();

				spriteComponent->texture = texture;
			}
		}
	}
	
	void AnimationSystem::Play(Entity* entity, std::string animationName)
	{
		Firelight::ECS::AnimationComponent* animatorComponent = entity->GetComponent<Firelight::ECS::AnimationComponent>();
		Firelight::ECS::SpriteComponent* spriteComponent = entity->GetComponent<Firelight::ECS::SpriteComponent>();
		if (animatorComponent == nullptr)
		{
			return;
		}

		if (m_animations.size() <= 0)
		{
			return;
		}

		if (m_animations.contains(animationName))
		{
			if (animatorComponent->currentAnimation != nullptr && 
				animatorComponent->currentAnimation->m_animationName == animationName &&
				animatorComponent->currentFrameIndex < animatorComponent->currentAnimation->m_frameCount - 1)
			{
				return;
			}
			animatorComponent->currentAnimation = m_animations[animationName];
			animatorComponent->currentFrameCount = 0.0f;
			animatorComponent->currentFrameIndex = 0;
			animatorComponent->shouldPlay = true;
		}
	}
}
