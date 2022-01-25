#include "UISystem.h"

#include "..\Source\Engine.h"


namespace Firelight::UI {
	
	UISystem::UISystem()
	{
		Initalize();
		
		AddWhitelistComponent<ECS::UIWidget>();

		
		
	}
	UISystem::~UISystem()
	{
	}
	void UISystem::Update(const Utils::Time& time)
	{
		if (DagItem != nullptr) {
			if (mouseRawCurr != Maths::Vec2f(0,0)) {
				DagItem->DefaultPosition = MousePosDrag;
			}
		}



		DockingSettings();
		
	}

	void UISystem::HandleEvents(void* data)
	{
		Firelight::Events::Input::MouseEvent* EventMouse = (Firelight::Events::Input::MouseEvent*)data;
		unsigned char* EventKey = (unsigned char*)data;
		Firelight::Events::Input::ControllerState* EventController = (Firelight::Events::Input::ControllerState*)data;
		
		if (EventKey == 0) {
			return;
		}

		if (EventMouse->GetType() == Events::Input::e_MouseEventType::Move) {
			float fx = ((EventMouse->GetMouseX() / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
			float fy = -((EventMouse->GetMouseY() / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;
			
			MousePosDrag = Maths::Vec3f(fx, fy, 0);
		}
		if (EventMouse->GetType() == Events::Input::e_MouseEventType::RawMove) {
			
			mouseRawCurr = Maths::Vec2f(EventMouse->GetMouseX(), EventMouse->GetMouseY());
			
		}
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			//checks
			ECS::UIWidget* UICom = m_entities[entityIndex]->GetComponent<ECS::UIWidget>();

			if (UICom->Textuer == nullptr || UICom->Transform == nullptr) {
				continue;
			}

			if (EventMouse->GetType() != Events::Input::e_MouseEventType::RawMove) 
			{
				if (DagItem == nullptr) {
					if (EventMouse->GetType() != Events::Input::e_MouseEventType::Move) {
						if (UICom->isPressable) {

							OnPress(EventMouse->GetMouseX(), EventMouse->GetMouseY(), EventMouse->GetType(), UICom);

						}
					}
					if (UICom->isHover) {
						OnHover(EventMouse->GetMouseX(), EventMouse->GetMouseY(), UICom);
					}
				}
					if (UICom->isDrag) {

						OnDrag(EventMouse->GetMouseX(), EventMouse->GetMouseY(), EventMouse->GetType(), UICom);
						
					}
					
				
			}
			if (EventMouse->GetType() == Events::Input::e_MouseEventType::Move && FocusedItem == UICom) {
				OnLeave(EventMouse->GetMouseX(), EventMouse->GetMouseY());
			}
		}
		
		
	}
	void UISystem::Initalize()
	{
		Events::EventDispatcher::AddListener<Events::Input::OnKeyPress>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseButtionPressEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseButtionReleaseEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseMoveEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseMoveRawEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::ContollerEvent>(this);
	}
	void UISystem::UpdateDocking()
	{
		DockingSettings();
	}
	
	
	void UISystem::OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget)
	{
		

		Maths::Rectf destRect(
			widget->Transform->position.x - widget->Transform->scale.x * 0.5f,
			widget->Transform->position.y - widget->Transform->scale.y * 0.5f,
			widget->Transform->scale.x, widget->Transform->scale.y);
	
		float fx= ((x / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
		float fy = -((y / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;

		

			if (fx >= destRect.x &&
				fx <= (destRect.x + destRect.w) &&
				fy >= destRect.y &&
				fy <= (destRect.y + destRect.h))
			{
				switch (mouseEvent)
				{
				case Firelight::Events::Input::e_MouseEventType::LRelease:
				{
					
					for (auto&& Event : widget->OnLeftPressFunctions)
					{
						Event();
					}
				}
					break;
				case Firelight::Events::Input::e_MouseEventType::RRelease:
					
					for (auto&& Event : widget->OnRightPressFunctions)
					{
						Event();
					}
					break;
				case Firelight::Events::Input::e_MouseEventType::MRelease:
					
					for (auto&& Event : widget->OnMiddlePressFunctions)
					{
						Event();
					}
					break;
		
				default:
					break;
				}

				
			
			}

	}
	void UISystem::OnLeave(int x, int y)
	{
		//check collison
		Maths::Rectf destRect(
			FocusedItem->Transform->position.x - FocusedItem->Transform->scale.x * 0.5f,
			FocusedItem->Transform->position.y - FocusedItem->Transform->scale.y * 0.5f,
			FocusedItem->Transform->scale.x, FocusedItem->Transform->scale.y);

		float fx = ((x / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
		float fy = -((y / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;

		if (!(fx >= destRect.x &&
			fx <= (destRect.x + destRect.w) &&
			fy >= destRect.y &&
			fy <= (destRect.y + destRect.h)))
		{
			ECS::UI_Button* Button = dynamic_cast<ECS::UI_Button*>(FocusedItem);
			if (Button == nullptr) {
				return;
			}
			if (Button->isChangeOfTex) {
				Button->Textuer->colour = Button->colour[0];
			}
			FocusedItem = nullptr;
			
		}

	}
	void UISystem::OnHover(int x, int y, ECS::UIWidget* widget)
	{

		float w = widget->Textuer->texture->GetDimensions().x * widget->Transform->scale.x;
		float h = widget->Textuer->texture->GetDimensions().y * widget->Transform->scale.y;
		Maths::Rectf destRect(
			widget->Transform->position.x,
			widget->Transform->position.y,
			widget->Textuer->texture->GetDimensions().x, widget->Textuer->texture->GetDimensions().y);

	/*	float fx = ((x / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
		float fy = -((y / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;*/

		if (x >= destRect.x &&
			x <= (destRect.x + destRect.w) &&
			y >= destRect.y &&
			y <= (destRect.y + destRect.h))
		{
			FocusedItem = widget;
			//do hover
			for (auto&& Event : widget->OnHoverFunctions)
			{
				Event();
			}

			ECS::UI_Button* Button = dynamic_cast<ECS::UI_Button*>(widget);
			if (Button == nullptr) {
				return;
			}
			if (Button->isChangeOfTex) {
				Button->Textuer->colour = Button->colour[1];
			}
		}
	}
	
	void UISystem::OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget)
	{
		time.Stop();

		float w = widget->Textuer->texture->GetDimensions().x * widget->Transform->scale.x;
		float h= widget->Textuer->texture->GetDimensions().y * widget->Transform->scale.y;
		Maths::Rectf destRect(
			widget->Transform->position.x - w* 0.5f,
			widget->Transform->position.y - h * 0.5f,
			w, h);

		float fx = ((x / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
		float fy = -((y / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;

		if (fx >= destRect.x &&
			fx <= (destRect.x + destRect.w) &&
			fy >= destRect.y &&
			fy <= (destRect.y + destRect.h))
		{
			
			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LPress && DagItem == nullptr) {
				
					//say item is being draged
					
					time.Start();
					press = true;
			}
			else if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease)
			{
				
				DagItem = nullptr;
				press = false;
			}
			 
			if (press && time.GetDurationSeconds() > 0.1f && DagItem == nullptr) {
				DagItem = widget;
			}
		
		}
		
	
	}
	void UISystem::OnNavergate()
	{
		//movethrough on focused
	}
	void UISystem::CheckChildern()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{


		}
		//move with parent
	}
	void UISystem::DockingSettings()
	{

		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			//checks
			auto* UICom = m_entities[entityIndex]->GetComponent<ECS::UIWidget, ECS::UI_Canvas>();
			if (UICom != nullptr) {
				switch (UICom->DockSettings)
				{
				case Firelight::ECS::e_AnchorSettings::TopRight:
					UICom->Transform->position.y = 1 - (UICom->Transform->scale.y * 0.5);
					UICom->Transform->position.x = 1;
					break;
				case Firelight::ECS::e_AnchorSettings::Top:
					UICom->Transform->position.y = 1 - (UICom->Transform->scale.y * 0.5);
					UICom->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::TopLeft:
					UICom->Transform->position.y = 1 - (UICom->Transform->scale.y * 0.5);
					UICom->Transform->position.x = -1;
					break;

				case Firelight::ECS::e_AnchorSettings::BottomRight:
					UICom->Transform->position.y = -1 + (UICom->Transform->scale.y * 0.5);
					UICom->Transform->position.x = 1;
					break;
				case Firelight::ECS::e_AnchorSettings::Bottom:
					UICom->Transform->position.y = -1 + (UICom->Transform->scale.y * 0.5);
					UICom->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::BottomLeft:
					UICom->Transform->position.y = -1 + (UICom->Transform->scale.y * 0.5);
					UICom->Transform->position.x = -1;
					break;

				case Firelight::ECS::e_AnchorSettings::Right:
					UICom->Transform->position.x = -1 + (UICom->Transform->scale.x * 0.5);
					UICom->Transform->position.y = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::Center:
					UICom->Transform->position.y = 0;
					UICom->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::Left:
					UICom->Transform->position.x = 1 - (UICom->Transform->scale.x * 0.5);
					UICom->Transform->position.y = 0;
					break;

				case Firelight::ECS::e_AnchorSettings::None:
					UICom->Transform->position = UICom->DefaultPosition;
					break;
				default:
					break;
				}
			}

			ECS::UI_Child* UIComa = m_entities[entityIndex]->GetComponent<ECS::UIWidget, ECS::UI_Child>();
			if (UIComa->Parent == nullptr) {
				switch (UIComa->DockSettings)
				{
				case Firelight::ECS::e_AnchorSettings::TopRight:
					UIComa->Transform->position.y = 1 - (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 1;
					break;
				case Firelight::ECS::e_AnchorSettings::Top:
					UIComa->Transform->position.y = 1 - (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::TopLeft:
					UIComa->Transform->position.y = 1 - (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = -1;
					break;

				case Firelight::ECS::e_AnchorSettings::BottomRight:
					UIComa->Transform->position.y = -1 + (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 1;
					break;
				case Firelight::ECS::e_AnchorSettings::Bottom:
					UIComa->Transform->position.y = -1 + (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::BottomLeft:
					UIComa->Transform->position.y = -1 + (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = -1;
					break;

				case Firelight::ECS::e_AnchorSettings::Right:
					UIComa->Transform->position.x = -1 + (UIComa->Transform->scale.x * 0.5);
					UIComa->Transform->position.y = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::Center:
					UIComa->Transform->position.y = 0;
					UIComa->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::Left:
					UIComa->Transform->position.x = 1 - (UIComa->Transform->scale.x * 0.5);
					UIComa->Transform->position.y = 0;
					break;

				case Firelight::ECS::e_AnchorSettings::None:
					UIComa->Transform->position = UIComa->DefaultPosition;
					break;
				default:
					break;
				}
			}
			else
			{
				switch (UIComa->DockSettings)
				{
				case Firelight::ECS::e_AnchorSettings::TopRight:
					UIComa->Transform->position.y = UIComa->Parent->Transform->position.y - (UIComa->Parent->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 1;
					break;
				case Firelight::ECS::e_AnchorSettings::Top:
					UIComa->Transform->position.y = 1 - (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::TopLeft:
					UIComa->Transform->position.y = 1 - (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = -1;
					break;

				case Firelight::ECS::e_AnchorSettings::BottomRight:
					UIComa->Transform->position.y = -1 + (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 1;
					break;
				case Firelight::ECS::e_AnchorSettings::Bottom:
					UIComa->Transform->position.y = -1 + (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::BottomLeft:
					UIComa->Transform->position.y = -1 + (UIComa->Transform->scale.y * 0.5);
					UIComa->Transform->position.x = -1;
					break;

				case Firelight::ECS::e_AnchorSettings::Right:
					UIComa->Transform->position.x = -1 + (UIComa->Transform->scale.x * 0.5);
					UIComa->Transform->position.y = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::Center:
					UIComa->Transform->position.y = 0;
					UIComa->Transform->position.x = 0;
					break;
				case Firelight::ECS::e_AnchorSettings::Left:
					UIComa->Transform->position.x = 1 - (UIComa->Transform->scale.x * 0.5);
					UIComa->Transform->position.y = 0;
					break;

				case Firelight::ECS::e_AnchorSettings::None:
					UIComa->Transform->position = UIComa->DefaultPosition;
					break;
				default:
					break;
				}
				
			}
			
			
		}
	}
}
