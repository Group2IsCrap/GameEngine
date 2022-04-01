#include "TextSystem.h"

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
	TextSystem::TextSystem()
	{
		AddWhitelistComponent<TextComponent>();

		Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&TextSystem::Render, this));
	}

	TextSystem::~TextSystem()
	{
	}

	void TextSystem::Render()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* transformComponent = m_entities[entityIndex]->GetComponent<TransformComponent>();
			auto* textComponent = m_entities[entityIndex]->GetComponent<TextComponent>();

			textComponent->text.SetAnchorPosition(Maths::Vec2f(transformComponent->position.x, transformComponent->position.y));

			if (!textComponent->text.GetMesh())
			{
				textComponent->text.UpdateMesh();
			}

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->DrawTextStandard(&(textComponent->text), textComponent->layer);
		}
	}
}
