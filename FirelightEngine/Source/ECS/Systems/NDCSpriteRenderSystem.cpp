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
		
		Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&NDCSpriteRenderSystem::Render, this));
	}

	NDCSpriteRenderSystem::~NDCSpriteRenderSystem()
	{
	}

	void NDCSpriteRenderSystem::Render()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			
			auto* nDCspriteComponent = m_entities[entityIndex]->GetComponent<NDCSpriteComponent>();

			Graphics::Texture* texture = nDCspriteComponent->texture;
			if (texture == nullptr)
			{
				texture = Graphics::AssetManager::Instance().GetDefaultTexture();
			}	

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->NDCDraw(nDCspriteComponent->destRect, texture, nDCspriteComponent->layer,(0,0), nDCspriteComponent->colour, nDCspriteComponent->sourceRect, nDCspriteComponent->flipX, nDCspriteComponent->flipY);

		}

	}
}