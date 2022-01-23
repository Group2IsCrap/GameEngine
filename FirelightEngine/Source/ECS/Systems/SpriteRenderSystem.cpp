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

		Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&SpriteRenderSystem::Render, this));
	}

	SpriteRenderSystem::~SpriteRenderSystem()
	{
	}

	void SpriteRenderSystem::Render()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* transformComponent = EntityComponentSystem::Instance()->GetComponent<TransformComponent>(m_entities[entityIndex]);
			auto* spriteComponent = EntityComponentSystem::Instance()->GetComponent<SpriteComponent>(m_entities[entityIndex]);

			// If we pass nullptr to WorldDraw() it will draw a coloured quad
			// Lets use the default texture instead
			Graphics::Texture* texture = spriteComponent->texture;
			if (texture == nullptr)
			{
				texture = Graphics::AssetManager::Instance().GetDefaultTexture();
			}

			const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / spriteComponent->pixelsPerUnit;

			Maths::Rectf destRect(
				transformComponent->position.x - spriteWorldSize.x * 0.5f,
				transformComponent->position.y - spriteWorldSize.y * 0.5f,
				spriteWorldSize.x, spriteWorldSize.y);

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, texture, spriteComponent->layer, (double)transformComponent->rotation, spriteComponent->colour, spriteComponent->sourceRect);
		}
	}
}
