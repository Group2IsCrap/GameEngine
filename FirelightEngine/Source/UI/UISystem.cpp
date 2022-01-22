#include "UISystem.h"
#include <typeinfo>


namespace Firelight::UI {
	
	UISystem::UISystem()
	{
		Initalize();

		//white list
		AddWhitelistComponent<ECS::UIWidget>();
	}
	UISystem::~UISystem()
	{
	}
	void UISystem::Update(double dt)
	{
		CheckChildern();
	}

	void UISystem::HandleEvents(void* data)
	{
		Firelight::Events::Input::MouseEvent* EventMouse = (Firelight::Events::Input::MouseEvent*)data;
		unsigned char* EventKey = (unsigned char*)data;
		Firelight::Events::Input::ControllerState* EventController = (Firelight::Events::Input::ControllerState*)data;

		
		if (!EventMouse->IsValid()) {
			return;
		}
		
		for (auto entitiy : m_entities ) {

			//checks
			auto* UICom = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIWidget>(entitiy);

			if (UICom->Textuer == nullptr || UICom->Transform == nullptr) {
				continue;
			}


			if (UICom->isPressable) {
				OnPress(EventMouse->GetMouseX(), EventMouse->GetMouseY(), EventMouse->GetType(), UICom);
			}
			if (UICom->isHover) {
				OnHover(EventMouse->GetMouseX(), EventMouse->GetMouseY());
			}
			if (UICom->isDrag) {
				OnDrag(EventMouse->GetMouseX(), EventMouse->GetMouseY(), EventMouse->GetType());
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
	
	
	void UISystem::OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* Wig)
	{
		
		
			const Maths::Vec2f topLeft = Maths::Vec2f(Wig->Transform->position.x, Wig->Transform->position.y) - Wig->Textuer->spriteDimensions * 0.5f;

			Maths::Rectf destRect(topLeft.x, topLeft.y, Wig->Textuer->spriteDimensions.x, Wig->Textuer->spriteDimensions.y);

			
			if (x >= destRect.x &&
				x <= (destRect.x + destRect.w) &&
				y >= destRect.y &&
				y <= (destRect.y + destRect.h)) 
			{
			
						
				switch (mouseEvent)
				{
				case Firelight::Events::Input::e_MouseEventType::LPress:
				{
					
					for (auto&& Event : Wig->OnLeftPressFunctions)
					{
						Event();
					}
				}
					break;
				case Firelight::Events::Input::e_MouseEventType::RPress:
					for (auto&& Event : Wig->OnRightPressFunctions)
					{
						Event();
					}
					break;
				case Firelight::Events::Input::e_MouseEventType::MPress:
					for (auto&& Event : Wig->OnMiddlePressFunctions)
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

	}
	void UISystem::OnHover(int x, int y)
	{
		//check mouse pos

	}
	
	void UISystem::OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent)
	{

		switch (mouseEvent)
		{
		case Firelight::Events::Input::e_MouseEventType::LPress:
			// is to be draged
			//complenet change pos
			break;
		case Firelight::Events::Input::e_MouseEventType::LRelease:
			//stop drag
			break;
		default:
			break;
		}

	}
	void UISystem::OnNavergate()
	{
		
		//movethrough on focused
	}
	void UISystem::CheckChildern()
	{
		//if not in canvas view do not render 
		//if not in pannle view do not render
		

	}
	void UISystem::DockingSettings()
	{

		for (auto entitiy : m_entities) {

			//checks
			auto* UICom = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UI_Canvas>(entitiy);
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
