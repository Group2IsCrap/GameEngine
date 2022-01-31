#include "HealthUI.h"

#include "Source/Graphics/AssetManager.h"

#include "../Events/HealthEvent.h"

HealthUI::HealthUI(Firelight::ECS::UIWidget* parent)
{
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::HealthEvent>(this);

	m_parentUI = parent;
	m_uiEntity = new Firelight::ECS::UIEntity();
	m_iconPath = "Sprites/UI/Heart.png";

	for (unsigned int i = 0; i < 3; ++i)
	{
		Firelight::ECS::UIEntity* heartUIEntity = new Firelight::ECS::UIEntity();
		heartUIEntity->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/UI/Heart.png");
		heartUIEntity->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Panel());
		Firelight::ECS::UI_Panel* panel = heartUIEntity->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>();
		panel->anchorSettings = Firelight::ECS::e_AnchorSettings::TopLeft;
		panel->offSet = Firelight::Maths::Vec2f((i * 75.0f), 10.0f);
		panel->parent = m_parentUI;
		panel->texture = heartUIEntity->GetSpriteComponent();
		panel->transform = heartUIEntity->GetTransformComponent();
		panel->defaultScale = Firelight::Maths::Vec3f(0.075f, 0.1f, 0);
		heartUIEntity->GetSpriteComponent()->toDraw = true;
		m_healthUIEntities.push_back(heartUIEntity);
	}
}

HealthUI::~HealthUI()
{
	delete m_uiEntity;
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
