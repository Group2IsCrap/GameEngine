#pragma once
#include"..\Events\Listener.h"
#include"..\ECS\Systems\System.h"

#include "../Events/Event.h"

//inputs
#include"..\Input\MouseEvent.h"
#include"..\Input\KeyboardEvent.h"
#include"..\Input\ControllerEvent.h"


#include"..\ECS\Components\UIComponents.h"
#include"..\ECS\Components\RenderingComponents.h"

namespace  Firelight::Events::UI {
	NEW_EVENT(BUTTION_PRESS)
}


namespace Firelight::UI {
	class UISystem :public Firelight::ECS::System, public Firelight::Events::Listener
	{
	public:
		UISystem();
		~UISystem();


		virtual void Update(double dt) override;


		
		void HandleEvents(void* data);

		/// <summary>
		/// subscribe to events
		/// </summary>
		void Initalize();


		void UpdateDocking();




		
		


	private:

		//Event function Invoke
		void OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* Wig);
		void OnLeave(int x, int y);
		void OnHover(int x, int y);
		void OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent);
		
		//move through items
		void OnNavergate();

		void CheckChildern();
		
		void DockingSettings();
	};


	
}