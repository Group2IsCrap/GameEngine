#include "UISystem.h"
#include"..\ECS\Components\UIComponents.h"
#include"..\ECS\Components\RenderingComponents.h"
namespace Firelight::UI {
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
		Firelight::Events::Input::KeyboardEvent* EventKey = (Firelight::Events::Input::KeyboardEvent*)data;
		Firelight::Events::Input::ControllerState* EventController = (Firelight::Events::Input::ControllerState*)data;

		
		if (!EventMouse->IsValid() || !EventKey->IsValid()) {
			return;
		}
		
		for (auto entitiy : Firelight::ECS::System::GetEntities()) {

			//checks
			


		}
	}
	void UISystem::Initalize()
	{
		Events::EventDispatcher::AddListener(Events::Input::OnKeyPress::sm_Description, this);
		Events::EventDispatcher::AddListener(Events::Input::MouseButtionPressEvent::sm_Description, this);
		Events::EventDispatcher::AddListener(Events::Input::MouseButtionReleaseEvent::sm_Description, this);
		Events::EventDispatcher::AddListener(Events::Input::MouseMoveEvent::sm_Description, this);
		Events::EventDispatcher::AddListener(Events::Input::MouseMoveRawEvent::sm_Description, this);
		Events::EventDispatcher::AddListener(Events::Input::ContollerEvent::sm_Description, this);
	}
	void UISystem::UpdateDocking()
	{
		DockingSettings();
	}
	void UISystem::OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent)
	{
		//invoke event
		switch (mouseEvent)
		{
		case Firelight::Events::Input::e_MouseEventType::LPress:
			
			break;
		case Firelight::Events::Input::e_MouseEventType::RPress:
			break;
		case Firelight::Events::Input::e_MouseEventType::MPress:
			break;
		
		default:
			break;
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

		for (auto entitiy : Firelight::ECS::System::GetEntities()) {

			//check
			if (!entitiy->Componets->hasChild) {
				continue;
			}
			for (auto ComponetsChild : entitiy->Child) {
				switch (entitiy->DockSettings)
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
