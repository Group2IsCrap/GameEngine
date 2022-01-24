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
				DagItem->Transform->position = MousePosDrag;
			}
		}
		
	}

	void UISystem::HandleEvents(void* data)
	{
		Firelight::Events::Input::MouseEvent* EventMouse = (Firelight::Events::Input::MouseEvent*)data;
		unsigned char* EventKey = (unsigned char*)data;
		Firelight::Events::Input::ControllerState* EventController = (Firelight::Events::Input::ControllerState*)data;
		
	

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

			if (EventMouse->GetType() != Events::Input::e_MouseEventType::RawMove) {


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
			
		}
		if (EventMouse->GetType() == Events::Input::e_MouseEventType::Move && FocusedItem != nullptr) {
			OnLeave(EventMouse->GetMouseX(), EventMouse->GetMouseY());
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
					FocusedItem = widget;
					for (auto&& Event : widget->OnLeftPressFunctions)
					{
						Event();
					}
				}
					break;
				case Firelight::Events::Input::e_MouseEventType::RRelease:
					FocusedItem = widget;
					for (auto&& Event : widget->OnRightPressFunctions)
					{
						Event();
					}
					break;
				case Firelight::Events::Input::e_MouseEventType::MRelease:
					FocusedItem = widget;
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

		if (fx <= destRect.x &&
			fx >= (destRect.x + destRect.w) &&
			fy <= destRect.y &&
			fy >= (destRect.y + destRect.h))
		{
			
			FocusedItem = nullptr;
		}

	}
	void UISystem::OnHover(int x, int y, ECS::UIWidget* widget)
	{

		Maths::Rectf destRect(
			widget->Transform->position.x - widget->Transform->scale.x * 0.5f,
			widget->Transform->position.y - widget->Transform->scale.y * 0.5f,
			widget->Transform->scale.x, widget->Transform->scale.y);

		float fx = ((x / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
		float fy = -((y / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;

		if (fx >= destRect.x &&
			fx <= (destRect.x + destRect.w) &&
			fy >= destRect.y &&
			fy <= (destRect.y + destRect.h))
		{


			//do hover
			for (auto&& Event : widget->OnHoverFunctions)
			{
				Event();
			}

		}
	}
	
	void UISystem::OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget)
	{

		Maths::Rectf destRect(
			widget->Transform->position.x - widget->Transform->scale.x* 0.5f,
			widget->Transform->position.y - widget->Transform->scale.y * 0.5f,
			widget->Transform->scale.x, widget->Transform->scale.y);

		float fx = ((x / Engine::Instance().GetWindowDimensionsFloat().x) - 0.5) * 2;
		float fy = -((y / Engine::Instance().GetWindowDimensionsFloat().y) - 0.5) * 2;

		if (fx >= destRect.x &&
			fx <= (destRect.x + destRect.w) &&
			fy >= destRect.y &&
			fy <= (destRect.y + destRect.h))
		{
			
			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LPress && DagItem == nullptr) {
				//say item is being draged
				DagItem = widget;
				FocusedItem = widget;
				
			}
			else if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease)
			{

				DagItem = nullptr;
				FocusedItem = nullptr;
				
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
			//checks
			ECS::UI_Canvas* UICom = m_entities[entityIndex]->GetComponent<ECS::UI_Canvas>();

			for (auto ComponetsChild : UICom->Child) {
				//if not in canvas view do not render 
				//if not in pannle view do not render
			}

		}

	}
	void UISystem::DockingSettings()
	{

		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			//checks
			auto* UICom = m_entities[entityIndex]->GetComponent<ECS::UI_Canvas>();
			//check
			if (!UICom->hasChild) {
				continue;
			}
			for (auto ComponetsChild : UICom->Child) {
				switch (UICom->DockSettings)
				{
				case Firelight::ECS::e_DockSettings::DockTop:
					break;
				case Firelight::ECS::e_DockSettings::DockBottom:
					break;
				case Firelight::ECS::e_DockSettings::DockCenter:
					break;	
				case Firelight::ECS::e_DockSettings::DockLeft:
						break;
				case Firelight::ECS::e_DockSettings::DockRight:
					break;
				case Firelight::ECS::e_DockSettings::DockNone:
					break;
				default:
					break;
				}
			}
		}
	}
}
