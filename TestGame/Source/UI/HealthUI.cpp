#include "HealthUI.h"

#include "Source/Graphics/AssetManager.h"

#include "../Events/HealthEvent.h"

HealthUI::HealthUI(Firelight::ECS::Entity* parent)
{
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::HealthEvent>(this);

	m_parentUI = parent;
	m_iconPath = "Sprites/UI/Heart.png";

	for (unsigned int i = 0; i < 3; ++i)
	{
		Firelight::ECS::UIPanel* heartUIEntity = new Firelight::ECS::UIPanel();
		heartUIEntity->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture(m_iconPath);
		heartUIEntity->GetSpriteComponent()->toDraw = true;
		heartUIEntity->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::TopLeft);
		heartUIEntity->SetOffset(Firelight::Maths::Vec2f((i * 75.0f), 10.0f));
		heartUIEntity->SetParent(m_parentUI->GetEntityID());
		heartUIEntity->SetDefaultDimensions(Firelight::Maths::Vec3f(96, 72, 0));
		m_healthUIEntities.push_back(heartUIEntity);
	}
}

HealthUI::~HealthUI()
{
}

void HealthUI::HandleEvents(const char* event, void* data)
{
	if (event != "HealthEvent")
		return;

	SetHealthAmount((int)data);
	
}

void HealthUI::SetHealthAmount(int health)
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		m_healthUIEntities[i]->GetSpriteComponent()->toDraw = health < i;
	}
}
