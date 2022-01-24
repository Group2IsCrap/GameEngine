#include "SpriteRenderSystem.h"

#include "../Components/BasicComponents.h"
#include "../Components/RenderingComponents.h"

#include "../../Graphics/GraphicsHandler.h"
#include "../../Graphics/SpriteBatch.h"
#include "../../Graphics/GraphicsEvents.h"
#include "../../Graphics/AssetManager.h"

#include "../EntityComponentSystem.h"

#include "../../Events/EventDispatcher.h"

namespace Firelight::ECS
{
	SpriteRenderSystem::SpriteRenderSystem()
	{
		AddWhitelistComponent<SpriteComponent>();
		AddWhitelistComponent<NDCSpriteComponent>();
		Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&SpriteRenderSystem::Render, this));
	}

	SpriteRenderSystem::~SpriteRenderSystem()
	{
	}

	void SpriteRenderSystem::Render()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* transformComponent = m_entities[entityIndex]->GetComponent<TransformComponent>();
			auto* spriteComponent = m_entities[entityIndex]->GetComponent<SpriteComponent>();

			//Draw in NDC space
			if (spriteComponent == nullptr) {
				auto* NDCspriteComponent = m_entities[entityIndex]->GetComponent<NDCSpriteComponent>();
				Maths::Rectf destRect(
					transformComponent->position.x - transformComponent->scale.x * 0.5f,
					transformComponent->position.y - transformComponent->scale.y * 0.5f,
					transformComponent->scale.x, transformComponent->scale.y);

				Graphics::GraphicsHandler::Instance().GetSpriteBatch()->NDCDraw(destRect, NDCspriteComponent->texture, NDCspriteComponent->layer, transformComponent->rotation, NDCspriteComponent->colour, NDCspriteComponent->sourceRect);
				continue;
			}

			// If we pass nullptr to WorldDraw() it will draw a coloured quad
			// Lets use the default texture instead
			Graphics::Texture* texture = spriteComponent->texture;
			if (texture == nullptr)
			{
				texture = Graphics::AssetManager::Instance().GetDefaultTexture();
			}

			const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / spriteComponent->pixelsPerUnit;

			Maths::Rectf destRect(
				transformComponent->position.x - spriteWorldSize.x * 0.5f + spriteComponent->drawOffset.x,
				transformComponent->position.y - spriteWorldSize.y * 0.5f + spriteComponent->drawOffset.y,
				spriteWorldSize.x, spriteWorldSize.y);

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, texture, spriteComponent->layer, (double)transformComponent->rotation, spriteComponent->colour, spriteComponent->sourceRect);
		}

		
	}
}
