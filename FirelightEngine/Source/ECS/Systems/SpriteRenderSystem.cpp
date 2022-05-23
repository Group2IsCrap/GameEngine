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
			auto* transformComponent = m_entities[entityIndex]->GetComponent<TransformComponent>();
			auto* spriteComponent = m_entities[entityIndex]->GetComponent<SpriteComponent>();

			// If we pass nullptr to WorldDraw() it will draw a coloured quad
			// Lets use the default texture instead
			Graphics::Texture* texture = spriteComponent->texture;
			if (texture == nullptr)
			{
				texture = Graphics::AssetManager::Instance().GetDefaultTexture();
			}

			const Maths::Vec2f spriteWorldSize = (!spriteComponent->sourceRect.IsValid()) ? Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / spriteComponent->pixelsPerUnit 
				: Maths::Vec2f(spriteComponent->sourceRect.w, spriteComponent->sourceRect.h) / spriteComponent->pixelsPerUnit;

			Firelight::Maths::Vec3f position = transformComponent->GetPosition();
			Maths::Rectf destRect(
				position.x - spriteWorldSize.x * 0.5f + spriteComponent->drawOffset.x,
				position.y - spriteWorldSize.y * 0.5f + spriteComponent->drawOffset.y,
				spriteWorldSize.x, spriteWorldSize.y);

			spriteComponent->flipX = transformComponent->GetFlipped();

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, texture, spriteComponent->layer, (double)transformComponent->GetRotation(), spriteComponent->colour, spriteComponent->sourceRect, spriteComponent->flipX, spriteComponent->flipY);
		}

		
	}
}
