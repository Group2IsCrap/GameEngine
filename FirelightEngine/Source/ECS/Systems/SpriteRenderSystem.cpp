#include "SpriteRenderSystem.h"

#include "../Components/BasicComponents.h"
#include "../Components/RenderingComponents.h"

#include "../../Graphics/GraphicsHandler.h"
#include "../../Graphics/SpriteBatch.h"
#include "../../Graphics/GraphicsEvents.h"

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

			const Maths::Vec2f topLeft = Maths::Vec2f(transformComponent->position.x, transformComponent->position.y) - spriteComponent->spriteDimensions * 0.5f;

			Maths::Rectf destRect(topLeft.x, topLeft.y, spriteComponent->spriteDimensions.x, spriteComponent->spriteDimensions.y);

			switch (spriteComponent->drawSpace)
			{
			case SpriteComponent::DrawSpace::e_NDC:
			{
				Graphics::GraphicsHandler::Instance().GetSpriteBatch()->NDCDraw(destRect, spriteComponent->texture, spriteComponent->layer, (double)transformComponent->rotation, spriteComponent->colour, spriteComponent->sourceRect);
			}
			break;
			case SpriteComponent::DrawSpace::e_Pixel:
			{
				Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(destRect, spriteComponent->texture, spriteComponent->layer, (double)transformComponent->rotation, spriteComponent->colour, spriteComponent->sourceRect);
			}
			break;
			case SpriteComponent::DrawSpace::e_World:
			{
				Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, spriteComponent->texture, spriteComponent->layer, (double)transformComponent->rotation, spriteComponent->colour, spriteComponent->sourceRect);
			}
			}
		}
	}
}
