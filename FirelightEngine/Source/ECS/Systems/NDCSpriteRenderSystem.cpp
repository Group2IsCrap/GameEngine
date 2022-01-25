#include "NDCSpriteRenderSystem.h"

#include "../Components/BasicComponents.h"
#include "../Components/RenderingComponents.h"

#include "../../Graphics/GraphicsHandler.h"
#include "../../Graphics/SpriteBatch.h"
#include "../../Graphics/GraphicsEvents.h"
#include "../../Graphics/AssetManager.h"

#include "../EntityComponentSystem.h"

#include "../../Events/EventDispatcher.h"

#include"../Source/Engine.h"
namespace Firelight::ECS
{
	NDCSpriteRenderSystem::NDCSpriteRenderSystem()
	{
		AddWhitelistComponent<NDCSpriteComponent>();
		AddWhitelistComponent<TransformComponent>();
		Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&NDCSpriteRenderSystem::Render, this));
	}

	NDCSpriteRenderSystem::~NDCSpriteRenderSystem()
	{
	}

	void NDCSpriteRenderSystem::Render()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* transformComponent = m_entities[entityIndex]->GetComponent<TransformComponent>();

			auto* NDCspriteComponent = m_entities[entityIndex]->GetComponent<NDCSpriteComponent>();

			Graphics::Texture* texture = NDCspriteComponent->texture;
			if (texture == nullptr)
			{
				texture = Graphics::AssetManager::Instance().GetDefaultTexture();
			}	
			
			Maths::Rectf destRect(
				transformComponent->position.x - transformComponent->scale.x,
				transformComponent->position.y - transformComponent->scale.y,
				transformComponent->scale.x, transformComponent->scale.y);

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->NDCDraw(destRect, texture, NDCspriteComponent->layer, transformComponent->rotation, NDCspriteComponent->colour, NDCspriteComponent->sourceRect);

		}

	}
}